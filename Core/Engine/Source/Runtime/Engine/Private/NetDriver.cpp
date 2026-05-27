#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/ReplicationDriver.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Parse.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Engine.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ChildConnection.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/WorldSettings.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetworkObjectList.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Public/NetworkingDistanceConstants.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameNetworkManager.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Pawn.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/PackageMapClient.h"

static int32* CVarSetNetDormancyEnabled = Finder::FindCVarDirect<int32>(L"net.DormancyEnable");
static int32 MaxConnectionsToTickPerServerFrame = 10; // its a cvar, idk how to get it so i just hardcoded it for now

static float ServerReplicateActorsTimeStart = 1.f;
static float ServerReplicateActorsTimeMs = 1.f;

FActorPriority::FActorPriority(UNetConnection* InConnection, UActorChannel* InChannel, FNetworkObjectInfo* InActorInfo, const TArray<struct FNetViewer>& Viewers, bool bLowBandwidth)
	: ActorInfo(InActorInfo), Channel(InChannel), DestructionInfo(NULL)
{
	if (!ActorInfo || !InConnection || !InChannel)
	{
		Priority = 0;
		return;
	}

	if (Version::Engine_Version <= 4.16 && Version::Engine_Version >= 4.24) {
		float Time = Channel ? (InConnection->Driver->Time - Channel->LastUpdateTime) : InConnection->Driver->SpawnPrioritySeconds;

		Priority = 0;
		for (int32 i = 0; i < Viewers.Num(); i++)
		{
			Priority = UKismetMathLibrary::Max(Priority, UKismetMathLibrary::Round(65536.0f * ActorInfo->Actor->GetNetPriority(Viewers[i].ViewLocation, Viewers[i].ViewDir, Viewers[i].InViewer, Viewers[i].ViewTarget, InChannel, Time, bLowBandwidth)));
		}
	}
}

FActorPriority::FActorPriority(class UNetConnection* InConnection, struct FActorDestructionInfo* Info, const TArray<struct FNetViewer>& Viewers)
	: ActorInfo(NULL), Channel(NULL), DestructionInfo(Info)
{

	Priority = 0;

	for (int32 i = 0; i < Viewers.Num(); i++)
	{
		float Time = InConnection->Driver->SpawnPrioritySeconds;

		FVector Dir = DestructionInfo->DestroyedPosition() - Viewers[i].ViewLocation;
		float DistSq = Dir.SizeSquared();

		if ((Viewers[i].ViewDir | Dir) < 0.f)
		{
			if (DistSq > NEARSIGHTTHRESHOLDSQUARED)
				Time *= 0.2f;
			else if (DistSq > CLOSEPROXIMITYSQUARED)
				Time *= 0.4f;
		}
		else if (DistSq > MEDSIGHTTHRESHOLDSQUARED)
			Time *= 0.4f;

		Priority = UKismetMathLibrary::Max(Priority, 65536.0f * Time);
	}
}

void UNetDriver::SetWorld(class UWorld* InWorld)
{
	if (Version::Engine_Version >= 4.26)
	{
		void (*SetWorldInternal)(UNetDriver*, UWorld*) = decltype(SetWorldInternal)(this->VTable[Finder::FindUNetDriver_SetWorldVFT()]);
		return SetWorldInternal(this, InWorld);
	}
	else
	{
		void (*SetWorldInternal)(UNetDriver*, UWorld*) = decltype(SetWorldInternal)(ImageBase + Finder::FindUNetDriver_SetWorld());
		return SetWorldInternal(this, InWorld);
	}
}

void UNetDriver::TickFlush(UNetDriver* This, float DeltaSeconds)
{
	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 5.0) {
		if (This->ClientConnections.Num() > 0 && This->ClientConnections[0]->InternalAck == false) {
			int32 Updated = This->ServerReplicateActors(DeltaSeconds);

			static int32 LastUpdateCount = 0;
			if ((LastUpdateCount && !Updated) || Updated)
			{
				//Log(This->NetDriverName.ToString().ToString() + " replicated " + std::to_string(Updated) + " actors");
			}
			LastUpdateCount = Updated;
		}
	}

	return TickFlushOG(This, DeltaSeconds);
}

void UNetDriver::SetNetDriverName(FName NewNetDriverNamed)
{
	NetDriverName = NewNetDriverNamed;
}

static FORCEINLINE bool IsActorRelevantToConnection(const AActor* Actor, const TArray<FNetViewer>& ConnectionViewers)
{
	for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.Num(); viewerIdx++)
	{
		FNetViewer Viewer = ConnectionViewers[viewerIdx];
		if (Actor->IsNetRelevantFor(Viewer.InViewer, Viewer.ViewTarget, Viewer.ViewLocation))
		{
			return true;
		}
	}

	return false;
}

bool UNetDriver::IsServer() const
{
	bool (*&IsServerInternal)(const UNetDriver*) = decltype(IsServerInternal)(VTable[Finder::FindUNetDriver_IsServerVFT()]);
	return IsServerInternal(this);
}

ENetMode UNetDriver::GetNetMode()
{
	ENetMode(*GetNetModeInternal)(UNetDriver*) = decltype(GetNetModeInternal)(ImageBase + Finder::FindUNetDriver_GetNetMode());
	return GetNetModeInternal(this);
}

bool UNetDriver::IsAdaptiveNetUpdateFrequencyEnabled()
{
	bool (*IsAdaptiveNetUpdateFrequencyEnabledInternal)() = decltype(IsAdaptiveNetUpdateFrequencyEnabledInternal)(ImageBase + Finder::FindUNetDriver_IsAdaptiveNetUpdateFrequencyEnabled());
	return IsAdaptiveNetUpdateFrequencyEnabledInternal();
}

static FORCEINLINE UNetConnection* IsActorOwnedByAndRelevantToConnection(const AActor* Actor, const TArray<FNetViewer>& ConnectionViewers, bool& bOutHasNullViewTarget)
{
	const AActor* ActorOwner = Actor->GetNetOwner();

	bOutHasNullViewTarget = false;

	for (int i = 0; i < ConnectionViewers.Num(); i++)
	{
		UNetConnection* ViewerConnection = ConnectionViewers[i].Connection;

		if (ViewerConnection->ViewTarget == nullptr)
		{
			bOutHasNullViewTarget = true;
		}

		if (ActorOwner == ViewerConnection->PlayerController ||
			(ViewerConnection->PlayerController && ActorOwner == ViewerConnection->PlayerController->K2_GetPawn()) ||
			(ViewerConnection->ViewTarget && ViewerConnection->ViewTarget->IsRelevancyOwnerFor(Actor, ActorOwner, ViewerConnection->OwningActor)))
		{
			return ViewerConnection;
		}
	}

	return nullptr;
}

static FORCEINLINE bool IsActorDormant(FNetworkObjectInfo* ActorInfo, const UNetConnection* Connection)
{
	if (!ActorInfo->DormantConnections.IsValid())
	{
		return false;
	}

	return ActorInfo->DormantConnections.Contains(TWeakObjectPtr<UNetConnection>(Connection));
}

static FORCEINLINE bool ShouldActorGoDormant(AActor* Actor, const TArray<FNetViewer>& ConnectionViewers, UActorChannel* Channel, const float Time, const bool bLowNetBandwidth)
{
	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 4.20) {
		UActorChannelUE416* ActorChannel = (UActorChannelUE416*)Channel;

		if (Actor->NetDormancy <= DORM_Awake || !ActorChannel || ActorChannel->bPendingDormancy || ActorChannel->Dormant)
		{
			return false;
		}

		if (Actor->NetDormancy == DORM_DormantPartial)
		{
			for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.Num(); viewerIdx++)
			{
				FNetViewer Viewer = ConnectionViewers[viewerIdx];
				if (!Actor->GetNetDormancy(Viewer.ViewLocation, Viewer.ViewDir, Viewer.InViewer, Viewer.ViewTarget, Channel, Time, bLowNetBandwidth))
				{
					return false;
				}
			}
		}

		return true;
	}

	return false;
}

bool UNetDriver::IsLevelInitializedForActor(const AActor* InActor, const UNetConnection* InConnection) const
{
	bool (*&IsLevelInitializedForActorInternal)(const UNetDriver*, const AActor*, const UNetConnection*) = decltype(IsLevelInitializedForActorInternal)(VTable[Finder::FindUNetDriver_IsLevelInitializedForActorVFT()]);
	return IsLevelInitializedForActorInternal(this, InActor, InConnection);
}

void UNetDriver::TickDispatch(UNetDriver* This, float DeltaTime)
{
	TickDispatchOG(This, DeltaTime);

	// Unless i can actually somehow make proper finders for everything in here then dont use
	//This->UpdateStandbyCheatStatus();
}

void UNetDriver::UpdateStandbyCheatStatus(void)
{
	if (ServerConnection == NULL && ClientConnections.Num())
	{
		if (bIsStandbyCheckingEnabled &&
			bHasStandbyCheatTriggered == false &&
			ClientConnections.Num() > 2)
		{
			int32 CountBadTx = 0;
			int32 CountBadRx = 0;
			int32 CountBadPing = 0;

			UWorld* FoundWorld = NULL;
			for (int32 Index = 0; Index < ClientConnections.Num(); Index++)
			{
				UNetConnection* NetConn = ClientConnections[Index];
				if (NetConn)
				{
					APlayerController* PlayerController = NetConn->PlayerController;
					if (PlayerController)
					{
						if (PlayerController->GetWorld() &&
							PlayerController->GetWorld()->TimeSeconds - PlayerController->CreationTime > JoinInProgressStandbyWaitTime &&
							PlayerController->IsPendingKillPending() == false)
						{
							if (!FoundWorld)
							{
								FoundWorld = PlayerController->GetWorld();
							}
							else
							{
								check(FoundWorld == PlayerController->GetWorld());
							}
							if (Time - NetConn->LastReceiveTime > StandbyRxCheatTime)
							{
								CountBadRx++;
							}
							if (Time - NetConn->LastRecvAckTime > StandbyTxCheatTime)
							{
								CountBadTx++;
							}
							if (PlayerController->PlayerState &&
								PlayerController->PlayerState->Ping * 4 > BadPingThreshold)
							{
								CountBadPing++;
							}
						}
					}
				}
			}

			if (FoundWorld)
			{
				AGameNetworkManager* const NetworkManager = FoundWorld->NetworkManager;
				if (NetworkManager)
				{
					if (float(CountBadRx) / float(ClientConnections.Num()) > PercentMissingForRxStandby)
					{
						bHasStandbyCheatTriggered = true;
						NetworkManager->StandbyCheatDetected(STDBY_Rx);
					}
					else if (float(CountBadPing) / float(ClientConnections.Num()) > PercentForBadPing)
					{
						bHasStandbyCheatTriggered = true;
						NetworkManager->StandbyCheatDetected(STDBY_BadPing);
					}
					else if (float(CountBadTx) / float(ClientConnections.Num()) > PercentMissingForTxStandby)
					{
						bHasStandbyCheatTriggered = true;
						NetworkManager->StandbyCheatDetected(STDBY_Tx);
					}
				}
			}
		}
	}
}

int32 UNetDriver::ServerReplicateActors(float DeltaSeconds)
{
	ServerReplicateActorsTimeStart = Utils::NowSeconds();

	UEngine* GEngine = UEngine::GetEngine();

	if (ClientConnections.Num() == 0)
	{
		return 0;
	}

	check(World);

	int32 Updated = 0;

	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 4.19) {
		ReplicationFrame++;

		const int32 NumClientsToTick = ServerReplicateActors_PrepConnections(DeltaSeconds);

		if (NumClientsToTick == 0)
		{
			return 0;
		}

		AWorldSettings* WorldSettings = World->GetWorldSettings();

		bool bCPUSaturated = false;
		float ServerTickTime = GEngine->GetMaxTickRate(DeltaSeconds);
		if (ServerTickTime == 0.f)
		{
			ServerTickTime = DeltaSeconds;
		}
		else
		{
			ServerTickTime = 1.f / ServerTickTime;
			bCPUSaturated = DeltaSeconds > 1.2f * ServerTickTime;
		}

		TArray<FNetworkObjectInfo*> ConsiderList;
		ConsiderList.Reserve(GetNetworkObjectList().GetActiveObjects().Num());

		ServerReplicateActors_BuildConsiderList(ConsiderList, ServerTickTime);

		for (int32 i = 0; i < ClientConnections.Num(); i++)
		{
			UNetConnection* Connection = ClientConnections[i];
			check(Connection);

			if (i >= NumClientsToTick)
			{
				for (int32 ConsiderIdx = 0; ConsiderIdx < ConsiderList.Num(); ConsiderIdx++)
				{
					FNetworkObjectInfo* ActorInfo = ConsiderList[ConsiderIdx];

					AActor* Actor = ActorInfo->Actor;
					if (Actor != NULL && !ActorInfo->bPendingNetUpdate)
					{
						UActorChannel* Channel = Connection->ActorChannels().FindRef(Actor);
						if (Channel != NULL && Channel->LastUpdateTime < ActorInfo->LastNetUpdateTime)
						{
							ActorInfo->bPendingNetUpdate = true;
						}
					}
				}
				Connection->TimeSensitive = false;
			}
			else if (Connection->ViewTarget)
			{
				TArray<FNetViewer>& ConnectionViewers = WorldSettings->ReplicationViewers;

				ConnectionViewers.Free();
				ConnectionViewers.Add(FNetViewer(Connection, DeltaSeconds));
				for (int32 ViewerIndex = 0; ViewerIndex < Connection->Children.Num(); ViewerIndex++)
				{
					if (Connection->Children[ViewerIndex]->ViewTarget != NULL)
					{
						ConnectionViewers.Add(FNetViewer(Connection->Children[ViewerIndex], DeltaSeconds));
					}
				}

				if (Connection->PlayerController)
				{
					Connection->PlayerController->SendClientAdjustment();
				}

				for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
				{
					if (Connection->Children[ChildIdx]->PlayerController != NULL)
					{
						Connection->Children[ChildIdx]->PlayerController->SendClientAdjustment();
					}
				}

				FActorPriority* PriorityList = NULL;
				FActorPriority** PriorityActors = NULL;

				const int32 FinalSortedCount = ServerReplicateActors_PrioritizeActors(Connection, ConnectionViewers, ConsiderList, bCPUSaturated, PriorityList, PriorityActors);

				const int32 LastProcessedActor = ServerReplicateActors_ProcessPrioritizedActors(Connection, ConnectionViewers, PriorityActors, FinalSortedCount, Updated);

				for (int32 k = LastProcessedActor; k < FinalSortedCount; k++)
				{
					if (!PriorityActors[k]->ActorInfo)
					{
						continue;
					}

					FNetworkObjectInfo* ActorInfo = PriorityActors[k]->ActorInfo;

					AActor* Actor = ActorInfo->Actor;

					UActorChannel* Channel = PriorityActors[k]->Channel;

					if (Channel != NULL && Time - Channel->RelevantTime <= 1.f)
					{
						ActorInfo->bPendingNetUpdate = true;
					}
					else if (IsActorRelevantToConnection(Actor, ConnectionViewers))
					{
						ActorInfo->bPendingNetUpdate = true;
						if (Channel != NULL)
						{
							Channel->RelevantTime = Time + 0.5f * UKismetMathLibrary::RandomFloat();
						}
					}
				}

				ConnectionViewers.Free();
			}
		}

		if (NumClientsToTick < ClientConnections.Num())
		{
			int32 NumConnectionsToMove = NumClientsToTick;
			while (NumConnectionsToMove > 0)
			{
				UNetConnection* Connection = ClientConnections[0];
				ClientConnections.RemoveAt(0, 1);
				ClientConnections.Add(Connection);
				NumConnectionsToMove--;
			}
		}
	}

	ServerReplicateActorsTimeMs = (Utils::NowSeconds() - ServerReplicateActorsTimeStart) * 1000.0;
	return Updated;
}

int32 UNetDriver::ServerReplicateActors_PrepConnections(const float DeltaSeconds)
{
	UEngine* GEngine = UEngine::GetEngine();

	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 5.3) {
		int32 NumClientsToTick = ClientConnections.Num();

		static bool bForceClientTickingThrottle = FParse::Param(GetCommandLineW(), TEXT("limitclientticks"));
		if (bForceClientTickingThrottle || GetNetMode() == NM_ListenServer)
		{
			static float DeltaTimeOverflow = 0.f;
			static bool LanPlay = FParse::Param(GetCommandLineW(), TEXT("lanplay"));

			float ClientUpdatesThisFrame = GEngine->NetClientTicksPerSecond * (DeltaSeconds + DeltaTimeOverflow) * (LanPlay ? 2.f : 1.f);
			NumClientsToTick = UKismetMathLibrary::Min(NumClientsToTick, UKismetMathLibrary::FTrunc(ClientUpdatesThisFrame));

			if (NumClientsToTick == 0)
			{
				DeltaTimeOverflow += DeltaSeconds;
				return 0;
			}
			DeltaTimeOverflow = 0.f;
		}

		if (MaxConnectionsToTickPerServerFrame > 0)
		{
			NumClientsToTick = UKismetMathLibrary::Min(ClientConnections.Num(), MaxConnectionsToTickPerServerFrame);
		} 

		bool bFoundReadyConnection = false;

		for (int32 ConnIdx = 0; ConnIdx < ClientConnections.Num(); ConnIdx++)
		{
			UNetConnection* Connection = ClientConnections[ConnIdx];
			check(Connection);
			check(Connection->State == USOCK_Pending || Connection->State == USOCK_Open || Connection->State == USOCK_Closed);
			checkSlow(Connection->GetUChildConnection() == NULL);

			AActor* OwningActor = Connection->OwningActor;
			if (OwningActor != NULL && Connection->State == USOCK_Open && (Connection->Driver->Time - Connection->LastReceiveTime < 1.5f))
			{
				if (World != OwningActor->GetWorld()) {
					continue;
				}

				bFoundReadyConnection = true;

				AActor* DesiredViewTarget = OwningActor;
				if (Connection->PlayerController)
				{
					if (AActor* ViewTarget = Connection->PlayerController->GetViewTarget())
					{
						if (ViewTarget->GetWorld())
						{
							DesiredViewTarget = ViewTarget;
						}
						else
						{
							Log("Player controller " + Connection->PlayerController->GetName().ToString() + "'s view target (" + ViewTarget->GetName().ToString() + ") no longer has a valid world! Was it unloaded as part a level instance?");
						}
					}
				}
				Connection->ViewTarget = DesiredViewTarget;

				for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
				{
					UNetConnection* Child = Connection->Children[ChildIdx];
					APlayerController* ChildPlayerController = Child->PlayerController;
					AActor* DesiredChildViewTarget = Child->OwningActor;

					if (ChildPlayerController)
					{
						AActor* ChildViewTarget = ChildPlayerController->GetViewTarget();

						if (ChildViewTarget && ChildViewTarget->GetWorld())
						{
							DesiredChildViewTarget = ChildViewTarget;
						}
					}

					Child->ViewTarget = DesiredChildViewTarget;
				}
			}
			else
			{
				Connection->ViewTarget = NULL;
				for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
				{
					Connection->Children[ChildIdx]->ViewTarget = NULL;
				}
			}
		}

		return bFoundReadyConnection ? NumClientsToTick : 0;
	}

	return 0;
}

void UNetDriver::ServerReplicateActors_BuildConsiderList(TArray<FNetworkObjectInfo*>& OutConsiderList, const float ServerTickTime)
{
	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 4.24) {
		int32 NumInitiallyDormant = 0;

		const bool bUseAdapativeNetFrequency = IsAdaptiveNetUpdateFrequencyEnabled();

		TArray<AActor*> ActorsToRemove;

		for (const TSharedPtr<FNetworkObjectInfo>& ObjectInfo : GetNetworkObjectList().GetActiveObjects())
		{
			FNetworkObjectInfo* ActorInfo = ObjectInfo.Get();

			if (!ActorInfo->bPendingNetUpdate && World->TimeSeconds <= ActorInfo->NextUpdateTime)
			{
				continue;
			}

			AActor* Actor = ActorInfo->Actor;

			if (Actor->IsPendingKillPending())
			{
				ActorsToRemove.Add(Actor);
				continue;
			}

			if (Actor->GetRemoteRole() == ROLE_None)
			{
				ActorsToRemove.Add(Actor);
				continue;
			}

			if (Actor->GetNetDriverName() != NetDriverName)
			{
				Log("Actor " + Actor->GetName().ToString() + " in wrong network actors list!");
				continue;
			}

			if (!Actor->IsActorInitialized())
			{
				continue;
			}

			ULevelUE416* Level = (ULevelUE416*)Actor->GetLevel();
			if (Level->HasVisibilityChangeRequestPending() || Level->bIsAssociatingLevel)
			{
				continue;
			}

			if (Actor->NetDormancy == DORM_Initial && Actor->IsNetStartupActor())
			{
				NumInitiallyDormant++;
				ActorsToRemove.Add(Actor);

				continue;
			}

			checkSlow(World == Actor->GetWorld());

			if (ActorInfo->LastNetReplicateTime == 0)
			{
				ActorInfo->LastNetReplicateTime = World->TimeSeconds;
				ActorInfo->OptimalNetUpdateDelta = 1.0f / Actor->NetUpdateFrequency;
			}

			const float ScaleDownStartTime = 2.0f;
			const float ScaleDownTimeRange = 5.0f;

			const float LastReplicateDelta = World->TimeSeconds - ActorInfo->LastNetReplicateTime;

			if (LastReplicateDelta > ScaleDownStartTime)
			{
				if (Actor->MinNetUpdateFrequency == 0.0f)
				{
					Actor->MinNetUpdateFrequency = 2.0f;
				}

				const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
				const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);

				const float Alpha = UKismetMathLibrary::Clamp((LastReplicateDelta - ScaleDownStartTime) / ScaleDownTimeRange, 0.0f, 1.0f);
				ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Lerp(MinOptimalDelta, MaxOptimalDelta, Alpha);
			}

			if (!ActorInfo->bPendingNetUpdate)
			{
				const float NextUpdateDelta = bUseAdapativeNetFrequency ? ActorInfo->OptimalNetUpdateDelta : 1.0f / Actor->NetUpdateFrequency;

				ActorInfo->NextUpdateTime = World->TimeSeconds + UKismetMathLibrary::RandomFloat() * ServerTickTime + NextUpdateDelta;

				ActorInfo->LastNetUpdateTime = Time;
			}

			ActorInfo->bPendingNetUpdate = false;

			OutConsiderList.Add(ObjectInfo.Get());

			Actor->CallPreReplication(this);
		}

		for (AActor* Actor : ActorsToRemove)
		{
			GetNetworkObjectList().Remove(Actor);
		}
	}
}

int32 UNetDriver::ServerReplicateActors_PrioritizeActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, const TArray<FNetworkObjectInfo*> ConsiderList, const bool bCPUSaturated, FActorPriority*& OutPriorityList, FActorPriority**& OutPriorityActors)
{
	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 4.19) {
		NetTag()++;
		Connection->TickCount++;

		for (int32 j = 0; j < Connection->SentTemporaries.Num(); j++)
		{
			Connection->SentTemporaries[j]->NetTag = NetTag();
		}

		check(World == Connection->OwningActor->GetWorld());

		int32 FinalSortedCount = 0;
		int32 DeletedCount = 0;

		TWeakObjectPtr<UNetConnection> WeakConnection(Connection);

		const int32 MaxSortedActors = ConsiderList.Num() + DestroyedStartupOrDormantActors().Num();
		if (MaxSortedActors > 0)
		{
			OutPriorityList = new FActorPriority[MaxSortedActors];
			OutPriorityActors = new FActorPriority * [MaxSortedActors];

			check(World == Connection->ViewTarget->GetWorld());

			AGameNetworkManager* const NetworkManager = World->NetworkManager;
			const bool bLowNetBandwidth = NetworkManager ? NetworkManager->IsInLowBandwidthMode() : false;

			for (FNetworkObjectInfo* ActorInfo : ConsiderList)
			{
				AActor* Actor = ActorInfo->Actor;

				UActorChannel* Channel = Connection->ActorChannels().FindRef(Actor);

				if (!Channel)
				{
					if (!IsLevelInitializedForActor(Actor, Connection))
					{
						continue;
					}

					if (!IsActorRelevantToConnection(Actor, ConnectionViewers))
					{
						continue;
					}
				}

				UNetConnection* PriorityConnection = Connection;

				if (Actor->bOnlyRelevantToOwner)
				{
					bool bHasNullViewTarget = false;

					PriorityConnection = IsActorOwnedByAndRelevantToConnection(Actor, ConnectionViewers, bHasNullViewTarget);

					if (PriorityConnection == nullptr)
					{
						if (!bHasNullViewTarget && Channel != NULL && Time - Channel->RelevantTime >= RelevantTimeout)
						{
							Channel->Close();
						}

						continue;
					}
				}
				/*else if (CVarSetNetDormancyEnabled && *CVarSetNetDormancyEnabled != 0) // This is broken for some reason
				{
					if (IsActorDormant(ActorInfo, Connection))
					{
						continue;
					}

					if (ShouldActorGoDormant(Actor, ConnectionViewers, Channel, Time, bLowNetBandwidth))
					{
						Channel->StartBecomingDormant();
					}
				}*/

				if (Actor->NetTag != NetTag())
				{
					Actor->NetTag = NetTag();

					OutPriorityList[FinalSortedCount] = FActorPriority(PriorityConnection, Channel, ActorInfo, ConnectionViewers, bLowNetBandwidth);
					OutPriorityActors[FinalSortedCount] = OutPriorityList + FinalSortedCount;

					FinalSortedCount++;
				}
			}

			for (int i = 0; i < Connection->DestroyedStartupOrDormantActors().Num(); i++)
			{
				FActorDestructionInfo* DInfo = DestroyedStartupOrDormantActors().Find(Connection->DestroyedStartupOrDormantActors()[i].Value);
				if (DInfo) {
					OutPriorityList[FinalSortedCount] = FActorPriority(Connection, DInfo, ConnectionViewers);
					OutPriorityActors[FinalSortedCount] = OutPriorityList + FinalSortedCount;
					FinalSortedCount++;
					DeletedCount++;
				}
			}

			std::sort(OutPriorityActors, OutPriorityActors + FinalSortedCount,
				[](const FActorPriority* A, const FActorPriority* B) {
					return A->Priority > B->Priority;
			});
		}

		return FinalSortedCount;
	}

	return 0;
}

int32 UNetDriver::ServerReplicateActors_ProcessPrioritizedActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, FActorPriority** PriorityActors, const int32 FinalSortedCount, int32& OutUpdated)
{
	if (Version::Engine_Version >= 4.16 && Version::Engine_Version <= 4.21) {
		int32 ActorUpdatesThisConnection = 0;
		int32 ActorUpdatesThisConnectionSent = 0;
		int32 FinalRelevantCount = 0;

		if (!Connection->IsNetReady(0))
		{
			return 0;
		}

		for (int32 j = 0; j < FinalSortedCount; j++)
		{
			if (PriorityActors[j]->ActorInfo == NULL && PriorityActors[j]->DestructionInfo)
			{
				if (PriorityActors[j]->DestructionInfo->StreamingLevelName() != UKismetStringLibrary::Conv_StringToName(L"None")
					&& !Connection->ClientVisibleLevelNames.Contains(PriorityActors[j]->DestructionInfo->StreamingLevelName()))
				{
					continue;
				}

				UActorChannel* Channel = (UActorChannel*)Connection->CreateChannel(CHTYPE_Actor, 1);
				if (Channel)
				{
					FinalRelevantCount++;

					Channel->SetChannelActorForDestroy(PriorityActors[j]->DestructionInfo);
					Connection->DestroyedStartupOrDormantActors().Remove(PriorityActors[j]->DestructionInfo->NetGUID());
				}
				continue;
			}

			UActorChannel* Channel = PriorityActors[j]->Channel;
			if (!Channel || Channel->Actor)
			{
				FNetworkObjectInfo* ActorInfo = PriorityActors[j]->ActorInfo;

				AActor* Actor = ActorInfo->Actor;
				bool bIsRelevant = false;

				const bool bLevelInitializedForActor = IsLevelInitializedForActor(Actor, Connection);

				if (bLevelInitializedForActor)
				{
					if (!Actor->bTearOff && (!Channel || Time - Channel->RelevantTime > 1.f))
					{
						if (IsActorRelevantToConnection(Actor, ConnectionViewers))
						{
							bIsRelevant = true;
						}
					}
				}
				else
				{
					Log("- Level not initialized for actor " + Actor->GetName().ToString());
				}

				const bool bIsRecentlyRelevant = bIsRelevant || (Channel && Time - Channel->RelevantTime < RelevantTimeout) || ActorInfo->bForceRelevantNextUpdate;

				ActorInfo->bForceRelevantNextUpdate = false;

				if (bIsRecentlyRelevant)
				{
					FinalRelevantCount++;

					if (Channel == NULL && GuidCache->SupportsObject(Actor->GetClass()) && GuidCache->SupportsObject(Actor->IsNetStartupActor() ? Actor : Actor->GetArchetype()))
					{
						if (bLevelInitializedForActor)
						{
							Channel = (UActorChannel*)Connection->CreateChannel(CHTYPE_Actor, 1);
							if (Channel)
							{
								Channel->SetChannelActor(Actor);
							}
						}
						else if (Actor->NetUpdateFrequency < 1.0f)
						{
							Log("Unable to replicate " + Actor->GetName().ToString());
							ActorInfo->NextUpdateTime = Actor->GetWorld()->TimeSeconds + 0.2f * UKismetMathLibrary::RandomFloat();
						}
					}

					if (Channel)
					{
						if (bIsRelevant)
						{
							Channel->RelevantTime = Time + 0.5f * UKismetMathLibrary::RandomFloat();
						}
						if (Channel->IsNetReady(0))
						{
							double ChannelLastNetUpdateTime = Channel->LastUpdateTime;

							if (Channel->ReplicateActor())
							{
								ActorUpdatesThisConnectionSent++;

								const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
								const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);
								const float DeltaBetweenReplications = (World->TimeSeconds - ActorInfo->LastNetReplicateTime);

								ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Clamp(DeltaBetweenReplications * 0.7f, MinOptimalDelta, MaxOptimalDelta);
								ActorInfo->LastNetReplicateTime = World->TimeSeconds;
							}
							ActorUpdatesThisConnection++;
							OutUpdated++;
						}
						else
						{
							Log("- Channel saturated, forcing pending update for " + Actor->GetName().ToString());
							Actor->ForceNetUpdate();
						}
						if (!Connection->IsNetReady(0))
						{
							return j;
						}
					}
				}

				if ((!bIsRecentlyRelevant || Actor->bTearOff) && Channel != NULL)
				{
					if (!bLevelInitializedForActor || !Actor->IsNetStartupActor())
					{
						//Log("- Closing channel for no longer relevant actor " + Actor->GetName().ToString());
						Channel->Close();
					}
				}
			}
		}

		return FinalSortedCount;
	}

	return 0;
}