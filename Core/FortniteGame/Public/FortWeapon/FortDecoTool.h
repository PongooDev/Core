#pragma once
#include "pch.h"

#include "FortWeapon.h"
#include "FortniteGame/Public/FortEnums.h"

class ABuildingSMActor;

class AFortDecoTool : public AFortWeapon {
public:
	DefineUnrealClass(AFortDecoTool);
public:
	static inline void (*ServerSpawnDecoOG)(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType);
	static void ServerSpawnDeco(AFortDecoTool* This, FVector& Location, FRotator& Rotation, ABuildingSMActor* AttachedActor, EBuildingAttachmentType InBuildingAttachmentType);

	static void Hook() {
		HookEveryVTable(
			AFortDecoTool::StaticClass(),
			AFortDecoTool::StaticClass()->GetFunction("Function /Script/FortniteGame.FortDecoTool.ServerSpawnDeco"),
			ServerSpawnDeco,
			(LPVOID*)&ServerSpawnDecoOG
		);
	}
};