#include "pch.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponent.h"

#include "FortniteGame/Public/Abilities/FortAbilitySet.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"
#include "FortniteGame/Public/Player/FortPlayerStateZone.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameStateZone.h"
#include "FortniteGame/Public/Missions/FortMissionManager.h"
#include "FortniteGame/Public/Missions/FortMissionLibrary.h"
#include "FortniteGame/Public/Abilities/FortGameplayAbility.h"
#include "FortniteGame/Public/Abilities/GameplayEffectApplicationInfo.h"

void UFortAbilitySystemComponent::GiveAbilitySet(UFortAbilitySet* AbilitySet) {
	if (!AbilitySet) {
		Log("UFortAbilitySystemComponent::GiveAbilitySet: AbilitySet is null!");
		return;
	}

	TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface = TScriptInterface<IAbilitySystemInterface>();
	AbilitySystemInterface.ObjectPointer = GetOwner();
	AbilitySystemInterface.InterfacePointer = GetOwner()->GetInterfaceAddress(IAbilitySystemInterface::StaticClass());

	FFortAbilitySetHandle AbilitySetHandle = UFortKismetLibrary::EquipFortAbilitySet(AbilitySystemInterface, AbilitySet, GetOwner());
	if (!AbilitySetHandle.TargetAbilitySystemComponent.Get()) {
		for (TSubclassOf<UFortGameplayAbility> GameplayAbility : AbilitySet->GameplayAbilities) {
			if (GameplayAbility.Get()) {
				FGameplayAbilitySpec AbilitySpec = *FGameplayAbilitySpec::ConstructAbilitySpec((UFortGameplayAbility*)GameplayAbility->GetDefaultObject(), 1, -1, nullptr);
				GiveAbility(AbilitySpec);
			}
		}
		if (AbilitySet->_HasGrantedGameplayEffects()) {
			for (int32 i = 0; i < AbilitySet->GrantedGameplayEffects.Num(); i++) {
				FGameplayEffectApplicationInfoHard& EffectInfo = AbilitySet->GrantedGameplayEffects.GetWithSize(i, FGameplayEffectApplicationInfoHard::GetSize());

				BP_ApplyGameplayEffectToSelf(EffectInfo.GameplayEffect.Get(), EffectInfo.Level, MakeEffectContext());
			}
		}
	}
}

void UFortAbilitySystemComponent::EndAllAbilities() {
	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability) {
			ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
			ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
			ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());
		}
	}
}

void UFortAbilitySystemComponent::EndDBNOAbilities() {
	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability) {
			UGameplayAbility* Ability = AbilitySpec.Ability;
			if (Ability && Utils::StringToLower(Ability->GetName().ToString()).contains("dbno")) {
				ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
				ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
				ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());
			}
		}
	}
}

void UFortAbilitySystemComponent::PrintAllAbilities() {
	Log("UFortAbilitySystemComponent::PrintAllAbilities: Printing all abilities for " + GetOwner()->GetName().ToString());
	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability) {
			Log(std::format("Ability {}: Name={}, Level={}, InputID={}, ActiveCount={}", i, AbilitySpec.Ability->GetName().ToString(), AbilitySpec.Level, AbilitySpec.InputID, AbilitySpec.ActiveCount));
		}
	}
}

void UFortAbilitySystemComponent::EndAllAbilitiesFromAbilitySet(UFortAbilitySet* AbilitySet) {
	if (!AbilitySet) {
		Log("UFortAbilitySystemComponent::EndAllAbilitiesFromAbilitySet: AbilitySet is null!");
		return;
	}

	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability) {
			UFortGameplayAbility* Ability = AbilitySpec.Ability->Cast<UFortGameplayAbility>();
			for (int32 j = 0; j < AbilitySet->GameplayAbilities.Num(); j++) {
				TSubclassOf<UFortGameplayAbility> GameplayAbility = AbilitySet->GameplayAbilities.GetWithSize(j);
				if (GameplayAbility.Get() == Ability->GetClass()) {
					ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
					ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
					ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());
					break;
				}
			}
		}
	}
}

void UFortAbilitySystemComponent::RemoveAllAbilitiesFromAbilitySet(UFortAbilitySet* AbilitySet) {
	if (!AbilitySet) {
		Log("UFortAbilitySystemComponent::RemoveAllAbilitiesFromAbilitySet: AbilitySet is null!");
		return;
	}

	bool bRemovedAny = false;

	for (int32 i = ActivatableAbilities.Items.Num() - 1; i >= 0; i--) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability) {
			UFortGameplayAbility* Ability = AbilitySpec.Ability->Cast<UFortGameplayAbility>();
			for (int32 j = 0; j < AbilitySet->GameplayAbilities.Num(); j++) {
				TSubclassOf<UFortGameplayAbility> GameplayAbility = AbilitySet->GameplayAbilities.GetWithSize(j);
				if (GameplayAbility.Get() == Ability->GetClass()) {
					ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
					ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
					ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());

					ActivatableAbilities.Items.RemoveAt(i, FGameplayAbilitySpec::GetSize());
					bRemovedAny = true;
					break;
				}
			}
		}
	}

	if (bRemovedAny) {
		ActivatableAbilities.MarkArrayDirty();
	}
}

void UFortAbilitySystemComponent::RemoveAllEffectsFromAbilitySet(UFortAbilitySet* AbilitySet) {
	if (!AbilitySet) {
		Log("UFortAbilitySystemComponent::RemoveAllEffectsFromAbilitySet: AbilitySet is null!");
		return;
	}

	if (AbilitySet->_HasGrantedGameplayEffects()) {
		for (int32 i = 0; i < AbilitySet->GrantedGameplayEffects.Num(); i++) {
			FGameplayEffectApplicationInfoHard& EffectInfo = AbilitySet->GrantedGameplayEffects.GetWithSize(i, FGameplayEffectApplicationInfoHard::GetSize());

			if (EffectInfo.GameplayEffect.Get()) {
				RemoveActiveGameplayEffectBySourceEffect(EffectInfo.GameplayEffect.Get());
			}
		}
	}

	if (AbilitySet->_HasPassiveGameplayEffects()) {
		for (int32 i = 0; i < AbilitySet->PassiveGameplayEffects.Num(); i++) {
			FGameplayEffectApplicationInfo& EffectInfo = AbilitySet->PassiveGameplayEffects.GetWithSize(i, FGameplayEffectApplicationInfo::GetSize());

			if (EffectInfo.GameplayEffect.Get()) {
				RemoveActiveGameplayEffectBySourceEffect(EffectInfo.GameplayEffect.Get());
			}
		}
	}
}

void UFortAbilitySystemComponent::EndAbility(UGameplayAbility* Ability) {
	if (!Ability) {
		Log("UFortAbilitySystemComponent::EndAbility: Ability is null!");
		return;
	}

	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (AbilitySpec.Ability == Ability) {
			ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
			ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
			ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());
			break;
		}
	}
}

void UFortAbilitySystemComponent::EndAbilitiesExcluding(TArray<TSubclassOf<UFortGameplayAbility>> AbilitiesToKeep) {
	for (int32 i = 0; i < ActivatableAbilities.Items.Num(); i++) {
		FGameplayAbilitySpec& AbilitySpec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());

		if (AbilitySpec.Ability) {
			UFortGameplayAbility* Ability = AbilitySpec.Ability->Cast<UFortGameplayAbility>();

			bool bShouldKeep = false;

			for (int32 j = 0; j < AbilitiesToKeep.Num(); j++) {
				TSubclassOf<UFortGameplayAbility> GameplayAbility = AbilitiesToKeep.GetWithSize(j, sizeof(TSubclassOf<UFortGameplayAbility>));

				if (GameplayAbility.Get() == Ability->GetClass()) {
					bShouldKeep = true;
					break;
				}
			}

			if (!bShouldKeep) {
				ClientCancelAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
				ClientEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo);
				ServerEndAbility(AbilitySpec.Handle, AbilitySpec.ActivationInfo, FPredictionKey());
			}
		}
	}
}

void UFortAbilitySystemComponent::RemoveAbilitySet(UFortAbilitySet* AbilitySet) {
	RemoveAllAbilitiesFromAbilitySet(AbilitySet);
	RemoveAllEffectsFromAbilitySet(AbilitySet);
}