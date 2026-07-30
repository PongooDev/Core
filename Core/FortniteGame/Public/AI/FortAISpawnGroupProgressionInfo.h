#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"

class UFortAISpawnGroup;

struct FSpawnGroupProgression {
public:
	DefineUnrealStruct(FSpawnGroupProgression);

	DefineStructProperty(UFortAISpawnGroup*, SpawnGroup);
public:
	uint8 Padding[0x8];
};

class UFortAISpawnGroupProgressionInfo : public UDataAsset {
public:
	DefineUnrealClass(UFortAISpawnGroupProgressionInfo);

	DefineUProperty(TArray<FSpawnGroupProgression>, SpawnGroups);
};
