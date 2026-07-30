#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Items/Definitions/FortResourceItemDefinition.h"
#include "FortniteGame/Public/Player/AthenaQuickChatActiveEntry.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetSerialization.h"

class ABuildingSMActor;

struct FBuildingWeakSpotData {
public:
	DefineUnrealStruct(FBuildingWeakSpotData);

	DefineStructProperty(TWeakObjectPtr<ABuildingSMActor>, ParentBuilding);
	DefineStructProperty(FVector_NetQuantizeNormal, Normal);
	DefineStructProperty(FVector_NetQuantize10, Position);
public:
	uint8 Padding[0x20];
};

class AFortBroadcastRemoteClientInfo : public AActor {
public:
	DefineUnrealClass(AFortBroadcastRemoteClientInfo);

	DefineUProperty(bool, bActive);
	DefineUProperty(bool, bRemoteIsInteracting);
	DefineUProperty(ABuildingSMActor*, RemoteEditActor);
	DefineUProperty(TArray<int32>, RemoteEditTileData);
	DefineUProperty(TSubclassOf<ABuildingSMActor>, RemoteBuildableClass);
	DefineUProperty(EFortResourceType, RemoteBuildingMaterial);
	DefineUProperty(bool, bRemoteIsFullScreenMapActive);
	DefineUProperty(bool, bRemoteIsInventoryActive);
	DefineUProperty(bool, bRemoteCanDBNORevive);
	DefineUProperty(FAthenaQuickChatActiveEntry, RemoteChatEntry);
	DefineUProperty(FBuildingWeakSpotData, RemoteWeakspotData);
	DefineUProperty(float, RemoteRespawnTime);
	DefineUProperty(int16, RemotePoiTagID);
	DefineUProperty(int32, RemoteEventScore);
};
