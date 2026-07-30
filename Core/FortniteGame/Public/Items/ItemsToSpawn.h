#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

class UFortWorldItemDefinition;

struct FItemsToSpawn {
public:
	DefineUnrealStruct(FItemsToSpawn);

	DefineStructProperty(UFortWorldItemDefinition*, ItemToDrop);
	DefineStructProperty(FScalableFloat, NumberToDrop);
public:
	uint8 Padding[0x28];
};
