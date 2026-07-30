#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortAISpawnGroup : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortAISpawnGroup);

	DefineUProperty(bool, bIsPrototype);
	DefineUProperty(bool, bIsValidForEnemySpawners);
	DefineUProperty(bool, bIsLargeSpawnGroup);
	DefineUProperty(float, MaxDiscountRatio);
	DefineUProperty(bool, bUseWeightSystem);
	DefineUProperty(FScalableFloat, Weight);
	DefineUProperty(FScalableFloat, MinNumber);
	DefineUProperty(FScalableFloat, MaxNumber);
	DefineUProperty(FCurveTableRowHandle, MaxGroupCategoryPopulationDensityCurve);
	DefineUProperty(FGameplayTagContainer, SpawnGroupGameplayTags);
	DefineUProperty(FGameplayTagQuery, RequiredTagQuery);
};
