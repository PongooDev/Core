#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"


class AFortPlayerPawn;

class ETInteractionType {
public:
	DefineUnrealEnum(ETInteractionType);

	DefineEnumProperty(IT_NoInteraction);
	DefineEnumProperty(IT_Simple);
	DefineEnumProperty(IT_LongPress);
	DefineEnumProperty(IT_BuildingEdit);
	DefineEnumProperty(IT_BuildingImprovement);
	DefineEnumProperty(IT_TrapPlacement);
};

struct FInteractionType {
public:
	DefineUnrealStruct(FInteractionType);

	DefineStructProperty(uint8, InteractionType);
	DefineStructProperty(TWeakObjectPtr<AFortPlayerPawn>, RequestingPawn);
public:
	uint8 Padding[0x38];
};