#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

class UAnimMontage;

struct FConsumeEffectData {
public:
	DefineUnrealStruct(FConsumeEffectData);

	DefineStructProperty(TSoftClassPtr<UClass>, EffectClass);
	DefineStructProperty(FScalableFloat, Level);
public:
	uint8 Padding[0x48];
};

class UFortConsumableItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortConsumableItemDefinition);

	DefineUProperty(TArray<FConsumeEffectData>, ConsumeEffects);
	DefineUProperty(TSoftObjectPtr<UAnimMontage>, UseAnimation);
	DefineUProperty(float, UseTime);
	DefineUProperty(bool, bRequiresMissingHealth);
};
