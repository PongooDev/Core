#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

struct FFortAIPawnUpgradeProbability {
public:
	DefineUnrealStruct(FFortAIPawnUpgradeProbability);

	DefineStructProperty(FGameplayTagQuery, TagQuery);
	DefineStructProperty(FCurveTableRowHandle, UpgradeProbability);
public:
	uint8 Padding[0x58];
};

class UFortAISpawnGroupUpgradeProbabilities : public UObject {
public:
	DefineUnrealClass(UFortAISpawnGroupUpgradeProbabilities);

	DefineUProperty(bool, bIsGuaranteedUpgrade);
	DefineUProperty(TArray<FFortAIPawnUpgradeProbability>, UpgradeProbabilities);
};
