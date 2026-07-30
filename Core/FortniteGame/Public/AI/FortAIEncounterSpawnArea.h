#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

#include "FortniteGame/Public/AI/FortAIEncounterRift.h"

class UFortPathCostEstimator;

struct FFortAIEncounterSpawnArea {
public:
	DefineUnrealStruct(FFortAIEncounterSpawnArea);

	DefineStructProperty(TArray<uint8>, QueryInstances);
	DefineStructProperty(TArray<FFortAIEncounterRift>, PendingRifts);
	DefineStructProperty(TArray<FFortAIEncounterRift>, Rifts);
	DefineStructProperty(TArray<UFortPathCostEstimator*>, PathEstimators);
	DefineStructProperty(bool, bIsActive);
	DefineStructProperty(bool, bUsingFallbackQuery);
	DefineStructProperty(uint8, SpawnLocationManagementMode);
public:
	uint8 Padding[0x48];
};
