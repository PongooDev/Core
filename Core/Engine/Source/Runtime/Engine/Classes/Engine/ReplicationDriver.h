#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AActor;
class UActorChannel;
class UNetConnection;
class UNetDriver;
class UReplicationDriver;
class UWorld;

struct FActorDestructionInfo;
struct FURL;

class UReplicationDriver : public UObject {
public:
	DefineUnrealClass(UReplicationDriver);
public:
	int32 ServerReplicateActors(float DeltaSeconds) {
		int32(*ServerReplicateActorsInternal)(UReplicationDriver * This, float DeltaSeconds) = decltype(ServerReplicateActorsInternal)(ImageBase + Finder::FindUReplicationDriver_ServerReplicateActors());
		return ServerReplicateActorsInternal(this, DeltaSeconds);
	}
};