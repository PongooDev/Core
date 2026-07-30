#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

class UEnvQuery;

struct FGoalSelectionQueryInfo {
public:
	DefineUnrealStruct(FGoalSelectionQueryInfo);

	DefineStructProperty(UEnvQuery*, GoalSelectionQuery);
	DefineStructProperty(FGameplayTagContainer, RequiredGameplayTags);
public:
	uint8 Padding[0x28];
};

class UFortAIAssignmentSettings : public UDataAsset {
public:
	DefineUnrealClass(UFortAIAssignmentSettings);

	DefineUProperty(TArray<FGoalSelectionQueryInfo>, GoalSelectionQueryInfos);
	DefineUProperty(bool, bGoalLocationsAlwaysKnown);
	DefineUProperty(bool, bIsEnemyAssignment);
	DefineUProperty(FGameplayTagContainer, RequiredTags);
	DefineUProperty(FGameplayTagContainer, ProhibitedTags);
	DefineUProperty(float, MaxAIAllowedForAssignment);
	DefineUProperty(float, MaxAIAllowedPerGoal);
};
