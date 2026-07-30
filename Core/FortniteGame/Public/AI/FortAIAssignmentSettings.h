#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UFortAIAssignmentSettings : public UDataAsset {
public:
	DefineUnrealClass(UFortAIAssignmentSettings);

	DefineUProperty(bool, bGoalLocationsAlwaysKnown);
	DefineUProperty(bool, bIsEnemyAssignment);
	DefineUProperty(FGameplayTagContainer, RequiredTags);
	DefineUProperty(FGameplayTagContainer, ProhibitedTags);
	DefineUProperty(float, MaxAIAllowedForAssignment);
	DefineUProperty(float, MaxAIAllowedPerGoal);
};
