#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"


struct FGhostModeRepData {
public:
	DefineUnrealStruct(FGhostModeRepData);

	DefineStructProperty(bool, bInGhostMode);
	DefineStructProperty(UFortWorldItemDefinition*, GhostModeItemDef);
	DefineStructProperty(int32, PreviousFocusedSlot);
	DefineStructProperty(float, TimeExitedGhostMode);
public:
	uint8 Padding[0x20];
};

