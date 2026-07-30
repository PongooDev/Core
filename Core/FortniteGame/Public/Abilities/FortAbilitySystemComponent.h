#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemComponent.h"

class UFortAbilitySet;
class UFortGameplayAbility;

class UFortAbilitySystemComponent : public UAbilitySystemComponent {
public:
	DefineUnrealClass(UFortAbilitySystemComponent);
public:
	void GiveAbilitySet(UFortAbilitySet* AbilitySet);

	void EndAllAbilities();

	void EndDBNOAbilities();

	void PrintAllAbilities();

	void EndAllAbilitiesFromAbilitySet(UFortAbilitySet* AbilitySet);

	void RemoveAllAbilitiesFromAbilitySet(UFortAbilitySet* AbilitySet);

	void RemoveAllEffectsFromAbilitySet(UFortAbilitySet* AbilitySet);

	void EndAbility(UGameplayAbility* Ability);

	void EndAbilitiesExcluding(TArray<TSubclassOf<UFortGameplayAbility>> AbilitiesToKeep);

	void RemoveAbilitySet(UFortAbilitySet* AbilitySet);
};
