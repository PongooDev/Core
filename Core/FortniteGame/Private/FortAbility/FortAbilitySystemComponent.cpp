#include "pch.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"

#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateZone.h"
#include "FortniteGame/Public/FortManager/FortMissionManager.h"
#include "FortniteGame/Public/Kismet/FortMissionLibrary.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"
#include "FortniteGame/Public/FortAbility/GameplayEffectApplicationInfo.h"

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