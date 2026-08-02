#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"
#include "Core/Public/Utils.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/EngineBaseTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "Engine/Source/Runtime/Core/Public/Misc/NetworkGuid.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class Error;
class FNetGUIDCache;
class FNetworkNotify;
class FNetworkObjectList;
class FObjectReplicator;
class FRepChangedPropertyTracker;
class FRepLayout;
class FReplicationChangelistMgr;
class FVoicePacket;
class StatelessConnectHandlerComponent;
class UNetConnection;
class UReplicationDriver;
struct FNetworkObjectInfo;
class UChannel;
class IAnalyticsProvider;
class FNetAnalyticsAggregator;
class UNetDriver;
class UWorld;

struct FActorPriority
{
	int32 Priority;	// Update priority, higher = more important.

	FNetworkObjectInfo* ActorInfo;	// Actor info.
	class UActorChannel* Channel;	// Actor channel.

	struct FActorDestructionInfo* DestructionInfo;	// Destroy an actor

	FActorPriority() :
		Priority(0), ActorInfo(NULL), Channel(NULL), DestructionInfo(NULL)
	{
	}

	FActorPriority(class UNetConnection* InConnection, class UActorChannel* InChannel, FNetworkObjectInfo* InActorInfo, const TArray<struct FNetViewer>& Viewers, bool bLowBandwidth);
	FActorPriority(class UNetConnection* InConnection, struct FActorDestructionInfo* DestructInfo, const TArray<struct FNetViewer>& Viewers);

	// Custom
	FActorPriority(struct FNetworkObjectInfo* InActorInfo, class UActorChannel* InChannel) :
		ActorInfo(InActorInfo),
		Channel(InChannel)
	{

	}
};

struct FActorDestructionInfo
{
public:
	TWeakObjectPtr<UObject> ObjOuter;
	FVector DestroyedPosition;
	uint32 NetGUID;
	FString PathName;
	FName StreamingLevelName;
};
static_assert(sizeof(FActorDestructionInfo) == 0x30, "Wrong size on FActorDestructionInfo");

class UNetDriver : public UObject {
public:
	DefineUnrealClass(UNetDriver);

	/** Used to specify the net driver to filter actors with (NAME_None || NAME_GameNetDriver is the default net driver) */
	DefineUProperty(FName, NetDriverName);

	DefineUProperty(int32, MaxInternetClientRate);

	DefineUProperty(int32, MaxClientRate);

	DefineUProperty(int32, NetServerMaxTickRate);

	DefineUProperty(float, ConnectionTimeout);

	DefineUProperty(float, InitialConnectTimeout);

	DefineUProperty(float, KeepAliveTime);

	/** Amount of time a server will wait before traveling to next map, gives clients time to receive final RPCs on existing level @see NextSwitchCountdown */
	DefineUProperty(float, ServerTravelPause);

	DefineUProperty(UReplicationDriver*, ReplicationDriver);

	DefineUProperty(UWorld*, World);

	DefineUProperty(UNetConnection*, ServerConnection);

	DefineUProperty(TArray<UNetConnection*>, ClientConnections);

	DefineCustomProperty(uint32, ReplicationFrame, ServerOffsets::UNetDriver__ReplicationFrame);

	DefineUProperty(float, Time);

	DefineCustomProperty(FNetworkObjectList*, NetworkObjects, ServerOffsets::UNetDriver__NetworkObjects);

	DefineUProperty(float, SpawnPrioritySeconds);

	DefineUProperty(float, RelevantTimeout);

	DefineCustomProperty(bool, DebugRelevantActors, ServerOffsets::UNetDriver__DebugRelevantActors);

	DefineCustomProperty(TArray<TWeakObjectPtr<AActor>>, LastPrioritizedActors, ServerOffsets::UNetDriver__LastPrioritizedActors);

	DefineCustomProperty(TArray<TWeakObjectPtr<AActor>>, LastRelevantActors, ServerOffsets::UNetDriver__LastRelevantActors);

	DefineCustomProperty(TArray<TWeakObjectPtr<AActor>>, LastSentActors, ServerOffsets::UNetDriver__LastSentActors);

	DefineCustomProperty(TArray<TWeakObjectPtr<AActor>>, LastNonRelevantActors, ServerOffsets::UNetDriver__LastNonRelevantActors);

	DefineCustomProperty(TSharedPtr<FNetGUIDCache>, GuidCache, ServerOffsets::UNetDriver__GuidCache);

	DefineCustomProperty(int32, SendCycles, ServerOffsets::UNetDriver__SendCycles);

	DefineCustomProperty(int32, RecvCycles, ServerOffsets::UNetDriver__RecvCycles);

	DefineCustomProperty(double, LastTickDispatchRealtime, ServerOffsets::UNetDriver__LastTickDispatchRealtime);

	DefineCustomProperty(bool, bIsStandbyCheckingEnabled, ServerOffsets::UNetDriver__bIsStandbyCheckingEnabled);

	DefineCustomProperty(bool, bHasStandbyCheatTriggered, ServerOffsets::UNetDriver__bHasStandbyCheatTriggered);

	DefineCustomProperty(float, JoinInProgressStandbyWaitTime, ServerOffsets::UNetDriver__JoinInProgressStandbyWaitTime);

	DefineCustomProperty(float, StandbyRxCheatTime, ServerOffsets::UNetDriver__StandbyRxCheatTime);

	DefineCustomProperty(float, StandbyTxCheatTime, ServerOffsets::UNetDriver__StandbyTxCheatTime);

	DefineCustomProperty(int32, BadPingThreshold, ServerOffsets::UNetDriver__BadPingThreshold);

	DefineCustomProperty(float, PercentMissingForRxStandby, ServerOffsets::UNetDriver__PercentMissingForRxStandby);

	DefineCustomProperty(float, PercentMissingForTxStandby, ServerOffsets::UNetDriver__PercentMissingForTxStandby);

	DefineCustomProperty(float, PercentForBadPing, ServerOffsets::UNetDriver__PercentForBadPing);

	DefineCustomProperty(int32, NetTag, ServerOffsets::UNetDriver__NetTag);
public:
	void SetWorld(UWorld* InWorld);

	bool InitListen(UWorld* InWorld, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error) {
		bool (*&InitListenInternal)(UNetDriver*, UWorld*, FURL&, bool, FString&) = decltype(InitListenInternal)(VTable[Finder::FindUNetDriver_InitListenVFT()]);
		return InitListenInternal(this, InWorld, ListenURL, bReuseAddressAndPort, Error);
	}

	static inline void (*TickFlushOG)(UNetDriver* This, float DeltaSeconds);
	static void TickFlush(UNetDriver* This, float DeltaSeconds);

	void SetNetDriverName(FName NewNetDriverNamed);

	bool IsServer() const;

	ENetMode GetNetMode();

	FNetworkObjectList& GetNetworkObjectList() { return *NetworkObjects; }
	const FNetworkObjectList& GetNetworkObjectList() const { return *NetworkObjects; }

	bool IsAdaptiveNetUpdateFrequencyEnabled();

	bool IsLevelInitializedForActor(const AActor* InActor, const UNetConnection* InConnection) const;

	static inline void (*TickDispatchOG)(UNetDriver* This, float DeltaTime);
	static void TickDispatch(UNetDriver* This, float DeltaTime);

	void UpdateStandbyCheatStatus(void);

	int32 ServerReplicateActors(float DeltaSeconds);

	int32 ServerReplicateActors_PrepConnections(const float DeltaSeconds);

	void ServerReplicateActors_BuildConsiderList(TArray<FNetworkObjectInfo*>& OutConsiderList, const float ServerTickTime);

	int32 ServerReplicateActors_PrioritizeActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, const TArray<FNetworkObjectInfo*> ConsiderList, const bool bCPUSaturated, FActorPriority*& OutPriorityList, FActorPriority**& OutPriorityActors);

	int32 ServerReplicateActors_ProcessPrioritizedActors(UNetConnection* Connection, const TArray<FNetViewer>& ConnectionViewers, FActorPriority** PriorityActors, const int32 FinalSortedCount, int32& OutUpdated);
public:
	using FDestructionInfoMap = TMap<FNetworkGUID, FActorDestructionInfo>;

	DefineCustomProperty(FDestructionInfoMap, DestroyedStartupOrDormantActors, ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetDriver_TickFlush()), TickFlush, (LPVOID*)&TickFlushOG);

		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetDriver_TickDispatch()), TickDispatch, (LPVOID*)&TickDispatchOG);

		Log("UNetDriver Hooked.");
	}
};