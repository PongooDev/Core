#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAbilitySystemComponent;
class UPackageMap;

struct FPredictionKey {
public:
	DefineUnrealStruct(FPredictionKey);

	int16 Current;
	int16 Base;
	uint8 Pad_4[0x4];
	UPackageMap* PredictiveConnection;
	bool bIsStale;
	bool bIsServerInitiated;
	uint8 Pad_12[0x6];
};