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

	DefineCustomProperty(float, LastRecvAckTime, ServerOffsets::UNetConnection__LastRecvAckTime);

	DefineUProperty(TArray<UChannel*>, OpenChannels);
public:
	int32 IsNetReady(bool Saturate);

	UChildConnection* GetUChildConnection()
	{
		UChildConnection* (*&GetUChildConnectionInternal)(UNetConnection * This) = decltype(GetUChildConnectionInternal)(VTable[Finder::FindUNetConnection_GetUChildConnectionVFT()]);
		return GetUChildConnectionInternal(this);
	}

	UChannel* CreateChannel(EChannelType Type, bool bOpenedLocally, int32 ChannelIndex = INDEX_NONE);

	void CleanUp();
public:
	TMap<TWeakObjectPtr<AActor>, UActorChannel*>& ActorChannels() {
		return *(TMap<TWeakObjectPtr<AActor>, UActorChannel*>*)((uintptr_t)this + ServerOffsets::UNetConnection__ActorChannels);
	}
	TSet<FNetworkGUID>& DestroyedStartupOrDormantActors() {
		return *(TSet<FNetworkGUID>*)((uintptr_t)this + ServerOffsets::UNetConnection__DestroyedStartupOrDormantActors);
	}
};