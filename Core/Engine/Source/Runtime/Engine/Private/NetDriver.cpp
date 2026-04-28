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

FActorPriority::FActorPriority(UNetConnection* InConnection, UActorChannel* InChannel, FNetworkObjectInfo* InActorInfo, const TArray<struct FNetViewer>& Viewers, bool bLowBandwidth)
	: ActorInfo(InActorInfo), Channel(InChannel), DestructionInfo(NULL)
{
	if (!ActorInfo || !InConnection || !InChannel)
	{
		Priority = 0;
		return;
	}

	if (Version::Engine_Version == 4.16) {
		float Time = Channel ? (InConnection->Driver->Time - Channel->LastUpdateTime) : InConnection->Driver->SpawnPrioritySeconds;
		// take the highest priority of the viewers on this connection
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

	if (Version::Engine_Version == 4.16) {
		Priority = 0;

		for (int32 i = 0; i < Viewers.Num(); i++)
		{
			float Time = InConnection->Driver->SpawnPrioritySeconds;

			FVector Dir = DestructionInfo->DestroyedPosition() - Viewers[i].ViewLocation;
			float DistSq = Dir.SizeSquared();

			// adjust priority based on distance and whether actor is in front of viewer
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
}

void UNetDriver::SetWorld(class UWorld* InWorld)
{
	void (*SetWorldInternal)(UNetDriver*, UWorld*) = decltype(SetWorldInternal)(ImageBase + Finder::FindUNetDriver_SetWorld());
	SetWorldInternal(this, InWorld);
}

void UNetDriver::TickFlush(UNetDriver* This, float DeltaSeconds)
{
	if (Version::Engine_Version == 4.16) {
		if (This->IsServer() && This->ClientConnections.Num() > 0 && This->ClientConnections[0]->InternalAck == false) {
			int32 Updated = This->ServerReplicateActors(DeltaSeconds);

			static int32 LastUpdateCount = 0;
			// Only log the zero replicated actors once after replicating an actor
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
	// 4.16 implementation
	NetDriverName = NewNetDriverNamed;
}

static FORCEINLINE bool IsActorRelevantToConnection(const AActor* Actor, const TArray<FNetViewer>& ConnectionViewers)
{
	if (!Actor) {
		return false;
	}

	if (Version::Engine_Version == 4.16) {
		if (!ConnectionViewers.IsValid()) {
			return false;
		}

		for (int32 viewerIdx = 0; viewerIdx < ConnectionViewers.Num(); viewerIdx++)
		{
			if (!ConnectionViewers.IsValidIndex(viewerIdx))
				continue;

			FNetViewer Viewer = ConnectionViewers[viewerIdx];
			if (Actor->IsNetRelevantFor(Viewer.InViewer, Viewer.ViewTarget, Viewer.ViewLocation))
			{
				return true;
			}
		}

		return false;
	}

	return false;
}

void UNetDriver::PrintDebugRelevantActors()
{

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
	if (Version::Engine_Version == 4.16) {
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

	return nullptr;
}

static FORCEINLINE bool IsActorDormant(FNetworkObjectInfo* ActorInfo, const UNetConnection* Connection)
{
	if (Version::Engine_Version == 4.16) {
		return ActorInfo->DormantConnections.Contains(TWeakObjectPtr<UNetConnection>(Connection));
	}

	return false;
}

// Returns true if this actor wants to go dormant for a particular connection
static FORCEINLINE bool ShouldActorGoDormant(AActor* Actor, const TArray<FNetViewer>& ConnectionViewers, UActorChannel* Channel, const float Time, const bool bLowNetBandwidth)
{
	if (Version::Engine_Version == 4.16) {
		UActorChannelUE416* ActorChannel = (UActorChannelUE416*)Channel;

		if (Actor->NetDormancy <= DORM_Awake || !ActorChannel || ActorChannel->bPendingDormancy || ActorChannel->Dormant)
		{
			// Either shouldn't go dormant, or is already dormant
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

	This->UpdateStandbyCheatStatus();
}

void UNetDriver::UpdateStandbyCheatStatus(void)
{
	if (Version::Engine_Version == 4.16) {
		// Only the server needs to check
		if (ServerConnection == NULL && ClientConnections.Num())
		{
			// Only check for cheats if enabled and one wasn't previously detected
			if (bIsStandbyCheckingEnabled &&
				bHasStandbyCheatTriggered == false &&
				ClientConnections.Num() > 2)
			{
				int32 CountBadTx = 0;
				int32 CountBadRx = 0;
				int32 CountBadPing = 0;

				UWorld* FoundWorld = NULL;
				// Look at each connection checking for a receive time and an ack time
				for (int32 Index = 0; Index < ClientConnections.Num(); Index++)
				{
					UNetConnection* NetConn = ClientConnections[Index];
					// Don't check connections that aren't fully formed (still loading & no controller)
					// Controller won't be present until the join message is sent, which is after loading has completed
					if (NetConn)
					{
						APlayerController* PlayerController = NetConn->PlayerController;
						if (PlayerController)
						{
							if (PlayerController->GetWorld() &&
								PlayerController->GetWorld()->TimeSeconds - PlayerController->CreationTime > JoinInProgressStandbyWaitTime &&
								// Ignore players with pending delete (kicked/timed out, but connection not closed)
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
								// Check for host tampering or crappy upstream bandwidth
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
						// See if we hit the percentage required for either TX or RX standby detection
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
						// Check for the host not sending to the clients
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
}

int32 UNetDriver::ServerReplicateActors(float DeltaSeconds)
{
	UEngine* GEngine = UEngine::GetEngine();

	if (Version::Engine_Version == 4.16) {
		if (ClientConnections.Num() == 0)
		{
			return 0;
		}

		check(World);

		int32 Updated = 0;

		// Bump the ReplicationFrame value to invalidate any properties marked as "unchanged" for this frame.
		ReplicationFrame++;

		const int32 NumClientsToTick = ServerReplicateActors_PrepConnections(DeltaSeconds);

		if (NumClientsToTick == 0)
		{
			// No connections are ready this frame
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

		// Build the consider list (actors that are ready to replicate)
		ServerReplicateActors_BuildConsiderList(ConsiderList, ServerTickTime);

		for (int32 i = 0; i < ClientConnections.Num(); i++)
		{
			UNetConnection* Connection = ClientConnections[i];
			check(Connection);

			// if this client shouldn't be ticked this frame
			if (i >= NumClientsToTick)
			{
				// then mark each considered actor as bPendingNetUpdate so that they will be considered again the next frame when the connection is actually ticked
				for (int32 ConsiderIdx = 0; ConsiderIdx < ConsiderList.Num(); ConsiderIdx++)
				{
					FNetworkObjectInfo* ActorInfo = ConsiderList[ConsiderIdx];

					AActor* Actor = ActorInfo->Actor;
					// if the actor hasn't already been flagged by another connection,
					if (Actor != NULL && !ActorInfo->bPendingNetUpdate)
					{
						// find the channel
						UActorChannel* Channel = Connection->ActorChannels().FindRef(Actor);
						// and if the channel last update time doesn't match the last net update time for the actor
						if (Channel != NULL && Channel->LastUpdateTime < ActorInfo->LastNetUpdateTime)
						{
							// flag it for a pending update
							ActorInfo->bPendingNetUpdate = true;
						}
					}
				}
				// clear the time sensitive flag to avoid sending an extra packet to this connection
				Connection->TimeSensitive = false;
			}
			else if (Connection->ViewTarget)
			{
				// Make a list of viewers this connection should consider (this connection and children of this connection)
				TArray<FNetViewer>& ConnectionViewers = WorldSettings->ReplicationViewers;

				ConnectionViewers.Reset();
				ConnectionViewers.Add(FNetViewer(Connection, DeltaSeconds));
				for (int32 ViewerIndex = 0; ViewerIndex < Connection->Children.Num(); ViewerIndex++)
				{
					if (Connection->Children[ViewerIndex]->ViewTarget != NULL)
					{
						ConnectionViewers.Add(FNetViewer(Connection->Children[ViewerIndex], DeltaSeconds));
					}
				}

				// send ClientAdjustment if necessary
				// we do this here so that we send a maximum of one per packet to that client; there is no value in stacking additional corrections
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

				// Get a sorted list of actors for this connection
				const int32 FinalSortedCount = ServerReplicateActors_PrioritizeActors(Connection, ConnectionViewers, ConsiderList, bCPUSaturated, PriorityList, PriorityActors);

				// Process the sorted list of actors for this connection
				const int32 LastProcessedActor = ServerReplicateActors_ProcessPrioritizedActors(Connection, ConnectionViewers, PriorityActors, FinalSortedCount, Updated);

				// relevant actors that could not be processed this frame are marked to be considered for next frame
				for (int32 k = LastProcessedActor; k < FinalSortedCount; k++)
				{
					if (!PriorityActors[k]->ActorInfo)
					{
						// A deletion entry, skip it because we dont have anywhere to store a 'better give higher priority next time'
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
						// If this actor was relevant but didn't get processed, force another update for next frame
						ActorInfo->bPendingNetUpdate = true;
						if (Channel != NULL)
						{
							Channel->RelevantTime = Time + 0.5f * UKismetMathLibrary::RandomFloat();
						}
					}
				}

				ConnectionViewers.Reset();
			}
		}

		// shuffle the list of connections if not all connections were ticked
		if (NumClientsToTick < ClientConnections.Num())
		{
			int32 NumConnectionsToMove = NumClientsToTick;
			while (NumConnectionsToMove > 0)
			{
				// move all the ticked connections to the end of the list so that the other connections are considered first for the next frame
				UNetConnection* Connection = ClientConnections[0];
				ClientConnections.RemoveAt(0, 1);
				ClientConnections.Add(Connection);
				NumConnectionsToMove--;
			}
		}

		if (DebugRelevantActors)
		{
			PrintDebugRelevantActors();
			LastPrioritizedActors.Empty();
			LastSentActors.Empty();
			LastRelevantActors.Empty();
			LastNonRelevantActors.Empty();

			DebugRelevantActors = false;
		}

		return Updated;
	}

	return 0;
}

int32 UNetDriver::ServerReplicateActors_PrepConnections(const float DeltaSeconds)
{
	UEngine* GEngine = UEngine::GetEngine();

	if (Version::Engine_Version == 4.16) {
		int32 NumClientsToTick = ClientConnections.Num();

		// by default only throttle update for listen servers unless specified on the commandline
		static bool bForceClientTickingThrottle = FParse::Param(GetCommandLineW(), TEXT("limitclientticks"));
		if (bForceClientTickingThrottle || GetNetMode() == NM_ListenServer)
		{
			// determine how many clients to tick this frame based on GEngine->NetTickRate (always tick at least one client), double for lan play
			static float DeltaTimeOverflow = 0.f;
			// updates are doubled for lan play
			static bool LanPlay = FParse::Param(GetCommandLineW(), TEXT("lanplay"));

			// come up with a solution that is greedier with higher framerates, but still won't risk saturating server upstream bandwidth
			float ClientUpdatesThisFrame = GEngine->NetClientTicksPerSecond * (DeltaSeconds + DeltaTimeOverflow) * (LanPlay ? 2.f : 1.f);
			NumClientsToTick = UKismetMathLibrary::Min(NumClientsToTick, UKismetMathLibrary::FTrunc(ClientUpdatesThisFrame));

			if (NumClientsToTick == 0)
			{
				// if no clients are ticked this frame accumulate the time elapsed for the next frame
				DeltaTimeOverflow += DeltaSeconds;
				return 0;
			}
			DeltaTimeOverflow = 0.f;
		}

		bool bFoundReadyConnection = false;

		for (int32 ConnIdx = 0; ConnIdx < ClientConnections.Num(); ConnIdx++)
		{
			UNetConnection* Connection = ClientConnections[ConnIdx];
			check(Connection);
			check(Connection->State == USOCK_Pending || Connection->State == USOCK_Open || Connection->State == USOCK_Closed);
			checkSlow(Connection->GetUChildConnection() == NULL);

			// Handle not ready channels.
			AActor* OwningActor = Connection->OwningActor;
			if (OwningActor != NULL && Connection->State == USOCK_Open && (Connection->Driver->Time - Connection->LastReceiveTime < 1.5f))
			{
				check(World == OwningActor->GetWorld());

				bFoundReadyConnection = true;

				// the view target is what the player controller is looking at OR the owning actor itself when using beacons
				Connection->ViewTarget = Connection->PlayerController ? Connection->PlayerController->GetViewTarget() : OwningActor;

				for (int32 ChildIdx = 0; ChildIdx < Connection->Children.Num(); ChildIdx++)
				{
					UNetConnection* Child = Connection->Children[ChildIdx];
					APlayerController* ChildPlayerController = Child->PlayerController;
					if (ChildPlayerController != NULL)
					{
						Child->ViewTarget = ChildPlayerController->GetViewTarget();
					}
					else
					{
						Child->ViewTarget = NULL;
					}
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
	if (Version::Engine_Version == 4.16) {
		int32 NumInitiallyDormant = 0;

		const bool bUseAdapativeNetFrequency = IsAdaptiveNetUpdateFrequencyEnabled();

		TArray<AActor*> ActorsToRemove;

		for (const TSharedPtr<FNetworkObjectInfo>& ObjectInfo : GetNetworkObjectList().GetActiveObjects())
		{
			FNetworkObjectInfo* ActorInfo = ObjectInfo.Get();

			if (!ActorInfo->bPendingNetUpdate && World->TimeSeconds <= ActorInfo->NextUpdateTime)
			{
				continue;		// It's not time for this actor to perform an update, skip it
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

			// This actor may belong to a different net driver, make sure this is the correct one
			// (this can happen when using beacon net drivers for example)
			if (Actor->GetNetDriverName() != NetDriverName)
			{
				Log("Actor " + Actor->GetName().ToString() + " in wrong network actors list!");
				continue;
			}

			// Verify the actor is actually initialized (it might have been intentionally spawn deferred until a later frame)
			if (!Actor->IsActorInitialized())
			{
				continue;
			}

			// Don't send actors that may still be streaming in or out
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

			// Set defaults if this actor is replicating for first time
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

				// Calculate min delta (max rate actor will update), and max delta (slowest rate actor will update)
				const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;									  // Don't go faster than NetUpdateFrequency
				const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta); // Don't go slower than MinNetUpdateFrequency (or NetUpdateFrequency if it's slower)

				// Interpolate between MinOptimalDelta/MaxOptimalDelta based on how long it's been since this actor actually sent anything
				const float Alpha = UKismetMathLibrary::Clamp((LastReplicateDelta - ScaleDownStartTime) / ScaleDownTimeRange, 0.0f, 1.0f);
				ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Lerp(MinOptimalDelta, MaxOptimalDelta, Alpha);
			}

			// Setup ActorInfo->NextUpdateTime, which will be the next time this actor will replicate properties to connections
			if (!ActorInfo->bPendingNetUpdate)
			{
				const float NextUpdateDelta = bUseAdapativeNetFrequency ? ActorInfo->OptimalNetUpdateDelta : 1.0f / Actor->NetUpdateFrequency;

				// then set the next update time
				ActorInfo->NextUpdateTime = World->TimeSeconds + UKismetMathLibrary::RandomFloat() * ServerTickTime + NextUpdateDelta;

				// and mark when the actor first requested an update
				ActorInfo->LastNetUpdateTime = Time;
			}

			// and clear the pending update flag assuming all clients will be able to consider it
			ActorInfo->bPendingNetUpdate = false;

			// add it to the list to consider below
			OutConsiderList.Add(ObjectInfo.Get());

			// Call PreReplication on all actors that will be considered
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
	if (Version::Engine_Version == 4.16) {
		// Get list of visible/relevant actors.

		NetTag()++;
		Connection->TickCount++;

		// Set up to skip all sent temporary actors
		for (int32 j = 0; j < Connection->SentTemporaries.Num(); j++)
		{
			Connection->SentTemporaries[j]->NetTag = NetTag();
		}

		// Make list of all actors to consider.
		check(World == Connection->OwningActor->GetWorld());

		int32 FinalSortedCount = 0;
		int32 DeletedCount = 0;

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

				UNetConnection* PriorityConnection = Connection;

				if (Actor->bOnlyRelevantToOwner)
				{
					// This actor should be owned by a particular connection, see if that connection is the one passed in
					bool bHasNullViewTarget = false;

					PriorityConnection = IsActorOwnedByAndRelevantToConnection(Actor, ConnectionViewers, bHasNullViewTarget);

					if (PriorityConnection == nullptr)
					{
						// Not owned by this connection, if we have a channel, close it, and continue
						if (!bHasNullViewTarget && Channel != NULL && Time - Channel->RelevantTime >= RelevantTimeout)
						{
							Log(std::format("Closing channel for: {}", Actor->GetName().ToString()));
							Channel->Close();
						}

						// This connection doesn't own this actor
						continue;
					}
				}
				else if (CVarSetNetDormancyEnabled && *CVarSetNetDormancyEnabled != 0)
				{
					// Skip Actor if dormant
					if (IsActorDormant(ActorInfo, Connection))
					{
						continue;
					}

					// See of actor wants to try and go dormant
					if (ShouldActorGoDormant(Actor, ConnectionViewers, Channel, Time, bLowNetBandwidth))
					{
						// Channel is marked to go dormant now once all properties have been replicated (but is not dormant yet)
						Channel->StartBecomingDormant();
					}
				}

				// Skip actor if not relevant and theres no channel already.
				// Historically Relevancy checks were deferred until after prioritization because they were expensive (line traces).
				// Relevancy is now cheap and we are dealing with larger lists of considered actors, so we want to keep the list of
				// prioritized actors low.
				if (!Channel)
				{
					if (!IsLevelInitializedForActor(Actor, Connection))
					{
						// If the level this actor belongs to isn't loaded on client, don't bother sending
						continue;
					}

					if (!IsActorRelevantToConnection(Actor, ConnectionViewers))
					{
						// If not relevant (and we don't have a channel), skip
						continue;
					}
				}

				// Actor is relevant to this connection, add it to the list
				if (Actor->NetTag != NetTag())
				{
					Actor->NetTag = NetTag();

					OutPriorityList[FinalSortedCount] = FActorPriority(PriorityConnection, Channel, ActorInfo, ConnectionViewers, bLowNetBandwidth);
					OutPriorityActors[FinalSortedCount] = OutPriorityList + FinalSortedCount;

					FinalSortedCount++;

					if (DebugRelevantActors)
					{
						LastPrioritizedActors.Add(Actor);
					}
				}
			}

			// Add in deleted actors
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

			// Sort by priority
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
	if (Version::Engine_Version == 4.16) {
		if (!Connection->IsNetReady(0))
		{
			// Connection saturated, don't process any actors
			return 0;
		}

		int32 ActorUpdatesThisConnection = 0;
		int32 ActorUpdatesThisConnectionSent = 0;
		int32 FinalRelevantCount = 0;

		for (int32 j = 0; j < FinalSortedCount; j++)
		{
			// Deletion entry
			if (PriorityActors[j]->ActorInfo == NULL && PriorityActors[j]->DestructionInfo)
			{
				// Make sure client has streaming level loaded
				if (PriorityActors[j]->DestructionInfo->StreamingLevelName() != UKismetStringLibrary::Conv_StringToName(L"None")
					&& !Connection->ClientVisibleLevelNames.Contains(PriorityActors[j]->DestructionInfo->StreamingLevelName()))
				{
					// This deletion entry is for an actor in a streaming level the connection doesn't have loaded, so skip it
					continue;
				}

				UActorChannel* Channel = (UActorChannel*)Connection->CreateChannel(CHTYPE_Actor, 1);
				if (Channel)
				{
					FinalRelevantCount++;

					Channel->SetChannelActorForDestroy(PriorityActors[j]->DestructionInfo);	// Send a close bunch on the new channel
					Connection->DestroyedStartupOrDormantActors().Remove(PriorityActors[j]->DestructionInfo->NetGUID()); // Remove from connections to-be-destroyed list (close bunch of reliable, so it will make it there)
				}
				continue;
			}

			// Normal actor replication
			UActorChannel* Channel = PriorityActors[j]->Channel;
			if (!Channel || Channel->Actor) //make sure didn't just close this channel
			{
				FNetworkObjectInfo* ActorInfo = PriorityActors[j]->ActorInfo;

				AActor* Actor = ActorInfo->Actor;
				bool bIsRelevant = false;

				const bool bLevelInitializedForActor = IsLevelInitializedForActor(Actor, Connection);

				// only check visibility on already visible actors every 1.0 + 0.5R seconds
				// bTearOff actors should never be checked
				if (bLevelInitializedForActor)
				{
					if (!Actor->bTearOff && (!Channel || Time - Channel->RelevantTime > 1.f))
					{
						if (IsActorRelevantToConnection(Actor, ConnectionViewers))
						{
							bIsRelevant = true;
						}
						else if (DebugRelevantActors)
						{
							LastNonRelevantActors.Add(Actor);
						}
					}
				}
				else
				{
					// Actor is no longer relevant because the world it is/was in is not loaded by client
					// exception: player controllers should never show up here
					Log("- Level not initialized for actor " + Actor->GetName().ToString());
				}

				// if the actor is now relevant or was recently relevant
				const bool bIsRecentlyRelevant = bIsRelevant || (Channel && Time - Channel->RelevantTime < RelevantTimeout);

				if (bIsRecentlyRelevant)
				{
					FinalRelevantCount++;

					// Find or create the channel for this actor.
					// we can't create the channel if the client is in a different world than we are
					// or the package map doesn't support the actor's class/archetype (or the actor itself in the case of serializable actors)
					// or it's an editor placed actor and the client hasn't initialized the level it's in
					if (Channel == NULL && GuidCache->SupportsObject(Actor->GetClass()) && GuidCache->SupportsObject(Actor->IsNetStartupActor() ? Actor : Actor->GetArchetype()))
					{
						if (bLevelInitializedForActor)
						{
							// Create a new channel for this actor.
							Channel = (UActorChannel*)Connection->CreateChannel(CHTYPE_Actor, 1);
							if (Channel)
							{
								Channel->SetChannelActor(Actor);
							}
						}
						// if we couldn't replicate it for a reason that should be temporary, and this Actor is updated very infrequently, make sure we update it again soon
						else if (Actor->NetUpdateFrequency < 1.0f)
						{
							Log("Unable to replicate " + Actor->GetName().ToString());
							ActorInfo->NextUpdateTime = Actor->GetWorld()->TimeSeconds + 0.2f * UKismetMathLibrary::RandomFloat();
						}
					}

					if (Channel)
					{
						// if it is relevant then mark the channel as relevant for a short amount of time
						if (bIsRelevant)
						{
							Channel->RelevantTime = Time + 0.5f * UKismetMathLibrary::RandomFloat();
						}
						// if the channel isn't saturated
						if (Channel->IsNetReady(0))
						{
							// replicate the actor
							if (DebugRelevantActors)
							{
								LastRelevantActors.Add(Actor);
							}

							if (Channel->ReplicateActor())
							{
								ActorUpdatesThisConnectionSent++;
								if (DebugRelevantActors)
								{
									LastSentActors.Add(Actor);
								}

								// Calculate min delta (max rate actor will upate), and max delta (slowest rate actor will update)
								const float MinOptimalDelta = 1.0f / Actor->NetUpdateFrequency;
								const float MaxOptimalDelta = UKismetMathLibrary::Max(1.0f / Actor->MinNetUpdateFrequency, MinOptimalDelta);
								const float DeltaBetweenReplications = (World->TimeSeconds - ActorInfo->LastNetReplicateTime);

								// Choose an optimal time, we choose 70% of the actual rate to allow frequency to go up if needed
								ActorInfo->OptimalNetUpdateDelta = UKismetMathLibrary::Clamp(DeltaBetweenReplications * 0.7f, MinOptimalDelta, MaxOptimalDelta);
								ActorInfo->LastNetReplicateTime = World->TimeSeconds;
							}
							ActorUpdatesThisConnection++;
							OutUpdated++;
						}
						else
						{
							Log("- Channel saturated, forcing pending update for " + Actor->GetName().ToString());
							// otherwise force this actor to be considered in the next tick again
							Actor->ForceNetUpdate();
						}
						// second check for channel saturation
						if (!Connection->IsNetReady(0))
						{
							// We can bail out now since this connection is saturated, we'll return how far we got though
							return j;
						}
					}
				}

				// If the actor wasn't recently relevant, or if it was torn off, close the actor channel if it exists for this connection
				if ((!bIsRecentlyRelevant || Actor->bTearOff) && Channel != NULL)
				{
					// Non startup (map) actors have their channels closed immediately, which destroys them.
					// Startup actors get to keep their channels open.

					if (!bLevelInitializedForActor || !Actor->IsNetStartupActor())
					{
						Log("- Closing channel for no longer relevant actor " + Actor->GetName().ToString());
						Channel->Close();
					}
				}
			}
		}

		return FinalSortedCount;
	}

	return 0;
}