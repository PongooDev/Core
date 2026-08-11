#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Player.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"
#include "Engine/Source/Runtime/Core/Public/Misc/NetworkGuid.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/Channel.h"

class FObjectReplicator;
class StatelessConnectHandlerComponent;
class UActorChannel;
class UChildConnection;
class AActor;
class UPackageMap;

enum { RELIABLE_BUFFER = 256 }; // Power of 2 >= 1.
enum { MAX_PACKETID = 16384 };  // Power of 2 >= 1, covering guaranteed loss/misorder time.
enum { MAX_CHSEQUENCE = 1024 }; // Power of 2 >RELIABLE_BUFFER, covering loss/misorder time.
enum { MAX_BUNCH_HEADER_BITS = 64 };
enum { MAX_PACKET_HEADER_BITS = 15 }; // = FMath::CeilLogTwo(MAX_PACKETID) + 1 (IsAck)
enum { MAX_PACKET_TRAILER_BITS = 1 };

class UNetDriver;

enum EConnectionState
{
	USOCK_Invalid = 0, // Connection is invalid, possibly uninitialized.
	USOCK_Closed = 1, // Connection permanently closed.
	USOCK_Pending = 2, // Connection is awaiting connection.
	USOCK_Open = 3, // Connection is open.
};

class UNetConnection : public UPlayer {
public:
	DefineUnrealClass(UNetConnection);

	DefineBitfieldUProperty(InternalAck);

	DefineUProperty(AActor*, ViewTarget);

	DefineUProperty(AActor*, OwningActor);

	DefineUProperty(UNetDriver*, Driver);

	DefineUProperty(double, LastReceiveTime);

	DefineUProperty(TArray<UChildConnection*>, Children);

	DefineCustomProperty(int32, TickCount, ServerOffsets::UNetConnection__TickCount);

	DefineCustomProperty(bool, TimeSensitive, ServerOffsets::UNetConnection__TimeSensitive);

	DefineUProperty(TArray<AActor*>, SentTemporaries);

	DefineCustomProperty(EConnectionState, State, ServerOffsets::UNetConnection__State);

	DefineCustomProperty(FName, ClientWorldPackageName, ServerOffsets::UNetConnection__ClientWorldPackageName);

	DefineCustomProperty(TArray<FName>, ClientVisibleLevelNames, ServerOffsets::UNetConnection__ClientVisibleLevelNames);

	DefineUProperty(UPackageMap*, PackageMap);

	DefineUProperty(int32, MaxPacket);

	DefineCustomProperty(float, LastRecvAckTime, ServerOffsets::UNetConnection__LastRecvAckTime);

	DefineUProperty(TArray<UChannel*>, OpenChannels);

public:
	static FORCEINLINE bool ClockBlockValid()
	{
		static int32 Cached = -1;

		if (Cached < 0)
		{
			const uintptr_t TickCount = ServerOffsets::UNetConnection__TickCount;
			const uintptr_t Reflected = UNetConnection::StaticClass()->GetPropertyOffset("LastReceiveTime");

			Cached = (TickCount > 0x2C && Reflected != (uintptr_t)-1 && TickCount - 0x2C == Reflected) ? 1 : 0;

			if (!Cached)
			{
				Log("UNetConnection clock block check failed - LastReceiveRealtime/LastGoodPacketRealtime/"
					"LastSendTime/LastTickTime/QueuedBits are unavailable on this build.");
			}
		}

		return Cached == 1;
	}

	static FORCEINLINE uintptr_t ClockOffset(int32 DeltaFromTickCount)
	{
		return ClockBlockValid() ? ServerOffsets::UNetConnection__TickCount + DeltaFromTickCount : 0;
	}

	DefineCustomProperty(double, LastReceiveRealtime, UNetConnection::ClockOffset(-0x24));
	DefineCustomProperty(double, LastGoodPacketRealtime, UNetConnection::ClockOffset(-0x1C));
	DefineCustomProperty(double, LastSendTime, UNetConnection::ClockOffset(-0x14));
	DefineCustomProperty(double, LastTickTime, UNetConnection::ClockOffset(-0x0C));
	DefineCustomProperty(int32, QueuedBits, UNetConnection::ClockOffset(-0x04));

	static FORCEINLINE uintptr_t ConnectTimeOffset()
	{
		const uintptr_t TickCount = ServerOffsets::UNetConnection__TickCount;
		const uintptr_t LastRecvAck = ServerOffsets::UNetConnection__LastRecvAckTime;

		if (!TickCount || !LastRecvAck || LastRecvAck != TickCount + 0x04)
			return 0;

		return LastRecvAck + 0x04;
	}

	DefineCustomProperty(float, ConnectTime, UNetConnection::ConnectTimeOffset());

	static FORCEINLINE uintptr_t MergeInfoOffset(int32 DeltaFromTimeSensitive)
	{
		const uintptr_t Base = ServerOffsets::UNetConnection__TimeSensitive;
		if (!Base)
			return 0;

		const int64 Result = (int64)Base + DeltaFromTimeSensitive;
		return Result > 0 ? (uintptr_t)Result : 0;
	}

	DefineCustomProperty(bool, AllowMerge, UNetConnection::MergeInfoOffset(-0x01));

	DefineCustomProperty(FOutBunch*, LastOutBunch, ServerOffsets::UNetConnection__TimeSensitive ? ((ServerOffsets::UNetConnection__TimeSensitive + 1 + 7) & ~(uintptr_t)7) : 0);
public:
	int32 IsNetReady(bool Saturate);

	UChildConnection* GetUChildConnection();

	static bool UsesChannelNames();

	static const FName& ChannelTypeToName(EChannelType ChType);

	UChannel* CreateChannelByName(const FName& ChName, uint32 CreateFlags, int32 ChannelIndex = INDEX_NONE);

	UChannel* CreateChannel(EChannelType Type, bool bOpenedLocally, int32 ChannelIndex = INDEX_NONE);

	void CleanUp();

	UActorChannel* FindActorChannelRef(const TWeakObjectPtr<AActor>& Actor)
	{
		if (Finder::FindUNetConnection_FindActorChannelRef()) {
			UActorChannel* (*FindActorChannelRefInternal)(UNetConnection*, const TWeakObjectPtr<AActor>&) = decltype(FindActorChannelRefInternal)(ImageBase + Finder::FindUNetConnection_FindActorChannelRef());
			return FindActorChannelRefInternal(this, Actor);
		}
		else {
			return ActorChannels.FindRef(Actor);
		}
	}
public:
	using FActorChannelMap = TMap<TWeakObjectPtr<AActor>, UActorChannel*>;

	DefineCustomProperty(FActorChannelMap, ActorChannels, ServerOffsets::UNetConnection__ActorChannels);
	DefineCustomProperty(TSet<FNetworkGUID>, DestroyedStartupOrDormantActors, ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors);
public:
	static inline void (*SendChallengeControlMessageOG)(UNetConnection* This, void* Response);
	static void SendChallengeControlMessage(UNetConnection* This, void* Response);

	static void Hook();
};