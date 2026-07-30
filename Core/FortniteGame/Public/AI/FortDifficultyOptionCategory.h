#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"

class UFortDifficultyOptionCategory : public UDataAsset {
public:
	DefineUnrealClass(UFortDifficultyOptionCategory);

	DefineUProperty(bool, bIsRequired);
	DefineUProperty(bool, bIsStatic);
	DefineUProperty(bool, bHasValueRange);
};
