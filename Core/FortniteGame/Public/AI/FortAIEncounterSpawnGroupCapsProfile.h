#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/AI/FortAIEncounterSpawnGroupCapsCategory.h"

struct FFortAIEncounterSpawnGroupCapsProfile {
public:
	DefineUnrealStruct(FFortAIEncounterSpawnGroupCapsProfile);

	DefineStructProperty(FGameplayTagContainer, EncounterTypeTags);
	DefineStructProperty(TArray<FFortAIEncounterSpawnGroupCapsCategory>, PopulationCategories);
public:
	uint8 Padding[0x30];
};
