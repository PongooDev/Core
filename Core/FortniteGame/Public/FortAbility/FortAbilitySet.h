#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

class UFortGameplayAbility;

struct FFortAbilitySetHandle {
public:
	DefineUnrealStruct(FFortAbilitySetHandle);

	uint8 Padding[0x28];
};

class UFortAbilitySet : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortAbilitySet);
public:
	DefineUProperty(TArray<TSubclassOf<UFortGameplayAbility>>, GameplayAbilities);
};