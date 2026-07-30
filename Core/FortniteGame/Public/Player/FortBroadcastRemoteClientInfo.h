#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Items/Definitions/FortResourceItemDefinition.h"
#include "FortniteGame/Public/Player/AthenaQuickChatActiveEntry.h"

class ABuildingSMActor;

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
	DefineUProperty(float, RemoteRespawnTime);
	DefineUProperty(int16, RemotePoiTagID);
	DefineUProperty(int32, RemoteEventScore);
};
