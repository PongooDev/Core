#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"


struct FGhostModeRepData {
public:
	DefineUnrealStruct(FGhostModeRepData);

	DefineStructProperty(bool, bInGhostMode);
	uint8 Pad_1[0x7];
	DefineStructProperty(UFortWorldItemDefinition*, GhostModeItemDef);
	DefineStructProperty(int32, PreviousFocusedSlot);
	DefineStructProperty(float, TimeExitedGhostMode);
};

