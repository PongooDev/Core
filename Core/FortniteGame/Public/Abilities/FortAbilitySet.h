#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "FortniteGame/Public/Abilities/GameplayEffectApplicationInfo.h"

class UFortGameplayAbility;
struct FGameplayEffectApplicationInfoHard;
struct FGameplayEffectApplicationInfo;
class UAbilitySystemComponent;

struct FFortAbilitySetHandle {
public:
	DefineUnrealStruct(FFortAbilitySetHandle);

	DefineStructProperty(TWeakObjectPtr<UAbilitySystemComponent>, TargetAbilitySystemComponent);

	uint8 Padding[0x28];
};

class UFortAbilitySet : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortAbilitySet);
public:
	DefineUProperty(TArray<TSubclassOf<UFortGameplayAbility>>, GameplayAbilities);
	DefineUProperty(TArray<FGameplayEffectApplicationInfoHard>, GrantedGameplayEffects);
	DefineUProperty(TArray<FGameplayEffectApplicationInfo>, PassiveGameplayEffects);
};