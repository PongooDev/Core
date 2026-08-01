#pragma once
#include "pch.h"

#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Athena/GhostModeRepData.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"


class AFortPlayerControllerGameplay : public AFortPlayerController {
public:
	DefineUnrealClass(AFortPlayerControllerGameplay);

	DefineUProperty(FGhostModeRepData, GhostModeRepData);

	DefineUProperty(FMulticastScriptDelegate, OnGhostModeChanged);
public:
	void CheckGhostModeItemReplicated();
	void CheckGhostModeItemRemoved(UFortWorldItemDefinition* GhostModeItemDef);

	static void Hook();
};