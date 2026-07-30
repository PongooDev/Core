#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortAISpawnGroupUpgrade : public UObject {
public:
	DefineUnrealClass(UFortAISpawnGroupUpgrade);

	DefineUProperty(FGameplayTagContainer, UpgradeTags);
	DefineUProperty(FGameplayTagQuery, GroupRequiredTagQuery);
	DefineUProperty(FName, AdditionalModifiersLootTierGroup);
	DefineUProperty(bool, bInvalidForEnemySpawners);
	DefineUProperty(float, SpawnGroupDiscountPercentage);
};
