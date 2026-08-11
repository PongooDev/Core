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
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

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
class ULevel;

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
	enum class ELayout : uint8 { Unknown = 0, WithLevel = 1, WithoutLevel = 2 };

	static inline ELayout Layout = ELayout::Unknown;

	static FORCEINLINE bool HasLevelField() { return Layout == ELayout::WithLevel; }
	static FORCEINLINE uint32 StructSize() { return HasLevelField() ? 0x38u : 0x30u; }

	static FORCEINLINE uint32 FieldOffset(uint32 WithLevel, uint32 WithoutLevel)
	{
		return HasLevelField() ? WithLevel : WithoutLevel;
	}

	static FORCEINLINE uint32 OffsetObjOuter() { return FieldOffset(0x08, 0x00); }
	static FORCEINLINE uint32 OffsetDestroyedPosition() { return FieldOffset(0x10, 0x08); }
	static FORCEINLINE uint32 OffsetNetGUID() { return FieldOffset(0x1C, 0x14); }
	static FORCEINLINE uint32 OffsetPathName() { return FieldOffset(0x20, 0x18); }
	static FORCEINLINE uint32 OffsetStreamingLevelName() { return FieldOffset(0x30, 0x28); }

private:
	uint8 OpaqueStorage[0x38];

public:
#define DefineDestructionInfoField(FieldType, FieldName) \
	FORCEINLINE FieldType& _Get##FieldName() \
	{ \
		return *reinterpret_cast<FieldType*>((uintptr_t)this + Offset##FieldName()); \
	} \
	FORCEINLINE const FieldType& _Get##FieldName() const \
	{ \
		return *reinterpret_cast<const FieldType*>((uintptr_t)this + Offset##FieldName()); \
	} \
	__declspec(property(get = _Get##FieldName)) FieldType FieldName;

	DefineDestructionInfoField(TWeakObjectPtr<UObject>, ObjOuter);
	DefineDestructionInfoField(FVector, DestroyedPosition);
	DefineDestructionInfoField(FNetworkGUID, NetGUID);
	DefineDestructionInfoField(FString, PathName);
	DefineDestructionInfoField(FName, StreamingLevelName);

#undef DefineDestructionInfoField
};
static_assert(sizeof(FActorDestructionInfo) == 0x38, "Wrong size on FActorDestructionInfo");

class FDestructionInfoMap
{
public:
	uint8* ElementsData;
	int32 ElementsArrayNum;
	int32 ElementsArrayMax;
	FBitArray ElementsAllocationFlags;
	int32 ElementsFirstFreeIndex;
	int32 ElementsNumFreeIndices;

	TInlineAllocator<1>::ForElementType<int32> Hash;
	int32 HashSize;

private:
	enum class EHashTrust : uint8 { Unknown, Trusted, Unreliable };
	static inline EHashTrust HashTrust = EHashTrust::Unknown;

	enum class EValueKind : uint8 { Unknown, Inline, Pointer };
	static inline EValueKind ValueKind = EValueKind::Unknown;

	static FORCEINLINE uint32 ValueOffset() { return 8; }
	static FORCEINLINE uint32 StrideFor(uint32 StructSize) { return 8 + StructSize + 8; }
	static FORCEINLINE uint32 StrideForPointer() { return 8 + 8 + 8; }

	static FORCEINLINE uint32 Stride()
	{
		return ValueKind == EValueKind::Pointer
			? StrideForPointer()
			: StrideFor(FActorDestructionInfo::StructSize());
	}

	FORCEINLINE FActorDestructionInfo* ValueAt(uint8* Element) const
	{
		if (ValueKind == EValueKind::Pointer)
			return *reinterpret_cast<FActorDestructionInfo**>(Element + ValueOffset());

		return reinterpret_cast<FActorDestructionInfo*>(Element + ValueOffset());
	}

	FORCEINLINE bool IsAllocated(int32 Index) const
	{
		return Index >= 0 && Index < ElementsArrayNum
			&& ElementsAllocationFlags.IsValidIndex(Index)
			&& (bool)ElementsAllocationFlags[Index];
	}

	FORCEINLINE uint8* ElementAt(int32 Index, uint32 InStride) const
	{
		return ElementsData + (size_t)Index * InStride;
	}

	static void ResolveLayout()
	{
		if (ValueKind != EValueKind::Unknown)
			return;

		const double V = Version::Engine_Version;

		const bool bUniquePtrValue = (V == 4.20 || V >= 4.21);
		const bool bHasLevelField = bUniquePtrValue || V >= 4.18;

		ValueKind = bUniquePtrValue ? EValueKind::Pointer : EValueKind::Inline;

		FActorDestructionInfo::Layout = bHasLevelField
			? FActorDestructionInfo::ELayout::WithLevel
			: FActorDestructionInfo::ELayout::WithoutLevel;
	}

	FORCEINLINE size_t SafeByteLimit() const
	{
		const uint32 KnownStride = (ValueKind != EValueKind::Unknown) ? Stride() : StrideForPointer();
		return (size_t)ElementsArrayMax * KnownStride;
	}

	static FORCEINLINE uint32 ValueSize()
	{
		return ValueKind == EValueKind::Pointer ? 8u : FActorDestructionInfo::StructSize();
	}

	static FORCEINLINE uint32 HashNextIdOffset() { return ValueOffset() + ValueSize(); }
	static FORCEINLINE uint32 HashIndexOffset() { return HashNextIdOffset() + 4; }

	FORCEINLINE bool IsElementInBounds(int32 Index, uint32 InStride) const
	{
		return (size_t)(Index + 1) * InStride <= SafeByteLimit();
	}

public:
	FORCEINLINE int32 NumAllocated() const { return ElementsArrayNum; }
	FORCEINLINE int32 Num() const { return ElementsArrayNum - ElementsNumFreeIndices; }

	FActorDestructionInfo* Find(const FNetworkGUID& Key) const
	{
		if (!ElementsData || ElementsArrayNum <= 0)
			return nullptr;

		ResolveLayout();

		const uint32 ElementStride = Stride();

		if (HashSize > 0 && (HashSize & (HashSize - 1)) == 0 && Hash.GetAllocation())
		{
			const uint32 BucketMask = (uint32)(HashSize - 1);
			const uint32 StoredBucketOffset = HashIndexOffset();

			if (HashTrust == EHashTrust::Unknown && HashSize > 1)
			{
				int32 Verified = 0;
				EHashTrust Result = EHashTrust::Trusted;

				for (int32 Index = 0; Index < ElementsArrayNum; ++Index)
				{
					if (!IsAllocated(Index) || !IsElementInBounds(Index, ElementStride))
						continue;

					const uint8* Element = ElementAt(Index, ElementStride);
					const uint32 ElementKey = *reinterpret_cast<const uint32*>(Element);
					const int32 StoredBucket = *reinterpret_cast<const int32*>(Element + StoredBucketOffset);

					if (StoredBucket != (int32)(GetTypeHash(FNetworkGUID(ElementKey)) & BucketMask))
					{
						Result = EHashTrust::Unreliable;
						break;
					}

					++Verified;
				}

				if (Result == EHashTrust::Unreliable || Verified > 0)
					HashTrust = Result;
			}

			if (HashTrust == EHashTrust::Trusted || HashSize == 1)
			{
				int32 Guard = ElementsArrayNum + 1;

				for (int32 Index = Hash.GetAllocation()[GetTypeHash(Key) & BucketMask];
					 Index != -1 && Guard-- > 0;
					 Index = *reinterpret_cast<const int32*>(ElementAt(Index, ElementStride) + HashNextIdOffset()))
				{
					if (!IsAllocated(Index) || !IsElementInBounds(Index, ElementStride))
						break;

					uint8* Element = ElementAt(Index, ElementStride);
					if (*reinterpret_cast<const uint32*>(Element) == Key.Value)
						return ValueAt(Element);
				}

				return nullptr;
			}
		}

		for (int32 Index = 0; Index < ElementsArrayNum; ++Index)
		{
			if (!IsAllocated(Index) || !IsElementInBounds(Index, ElementStride))
				continue;

			uint8* Element = ElementAt(Index, ElementStride);
			if (*reinterpret_cast<const uint32*>(Element) == Key.Value)
				return ValueAt(Element);
		}

		return nullptr;
	}
};

static_assert(sizeof(FDestructionInfoMap) == 0x50, "FDestructionInfoMap must match TSet's 0x50 layout");
static_assert(offsetof(FDestructionInfoMap, ElementsAllocationFlags) == 0x10, "AllocationFlags must sit at 0x10 like TSparseArray");
static_assert(offsetof(FDestructionInfoMap, ElementsFirstFreeIndex) == 0x30, "FirstFreeIndex must sit at 0x30 like TSparseArray");
static_assert(offsetof(FDestructionInfoMap, Hash) == 0x38, "Hash must sit at 0x38 like TSet");
static_assert(offsetof(FDestructionInfoMap, HashSize) == 0x48, "HashSize must sit at 0x48 like TSet");

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

	static FORCEINLINE bool ClockBlockValid()
	{
		static int32 Cached = -1;

		if (Cached < 0)
		{
			const uintptr_t Realtime = ServerOffsets::UNetDriver__LastTickDispatchRealtime;
			const uintptr_t Send = ServerOffsets::UNetDriver__SendCycles;

			Cached = (Realtime > 0x10 && (!Send || Send == Realtime + 0x0C)) ? 1 : 0;

			if (!Cached)
			{
				Log("UNetDriver clock block check failed - ElapsedTime/bIsPeer/ProfileStats/"
					"bSkipLocalStats are unavailable on this build.");
			}
		}

		return Cached == 1;
	}

	static FORCEINLINE uintptr_t ClockOffset(int32 DeltaFromLastTickDispatchRealtime)
	{
		if (!ClockBlockValid())
			return 0;

		const int64 Result = (int64)ServerOffsets::UNetDriver__LastTickDispatchRealtime + DeltaFromLastTickDispatchRealtime;
		return Result > 0 ? (uintptr_t)Result : 0;
	}

	static FORCEINLINE uintptr_t ElapsedTimeOffset()
	{
		static uintptr_t Cached = (uintptr_t)-1;

		if (Cached == (uintptr_t)-1)
		{
			Cached = 0;

			const uintptr_t Reflected = UNetDriver::StaticClass()->GetPropertyOffset("Time");
			const uintptr_t Realtime = ServerOffsets::UNetDriver__LastTickDispatchRealtime;

			if (Reflected != (uintptr_t)-1 && Reflected > 0 && Realtime > 0)
			{
				const uintptr_t AfterTime = (Reflected + sizeof(float) + 7) & ~(uintptr_t)7;
				if (Realtime == AfterTime + 0x08)
					Cached = AfterTime;
			}
		}

		return Cached;
	}

	DefineCustomProperty(double, ElapsedTime, UNetDriver::ElapsedTimeOffset());

	DefineCustomProperty(bool, bIsPeer, UNetDriver::ClockOffset(0x08));
	DefineCustomProperty(bool, ProfileStats, UNetDriver::ClockOffset(0x09));

	DefineCustomProperty(bool, bSkipLocalStats, ServerOffsets::UNetDriver__SendCycles ? UNetDriver::ClockOffset(0x0A) : 0);

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
	DefineCustomProperty(FDestructionInfoMap, DestroyedStartupOrDormantActors, ServerOffsets::UNetDriver__DestroyedStartupOrDormantActors);
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetDriver_TickFlush()), TickFlush, (LPVOID*)&TickFlushOG);

		MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetDriver_TickDispatch()), TickDispatch, (LPVOID*)&TickDispatchOG);

		Log("UNetDriver Hooked.");
	}
};