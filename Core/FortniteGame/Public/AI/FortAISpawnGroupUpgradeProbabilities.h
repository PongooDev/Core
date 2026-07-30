#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortAISpawnGroupUpgradeProbabilities : public UObject {
public:
	DefineUnrealClass(UFortAISpawnGroupUpgradeProbabilities);

	DefineUProperty(bool, bIsGuaranteedUpgrade);
};
