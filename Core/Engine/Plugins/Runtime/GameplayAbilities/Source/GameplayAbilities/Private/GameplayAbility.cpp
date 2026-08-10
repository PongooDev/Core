#include "pch.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/Abilities/GameplayAbility.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemLog.h"

bool UGameplayAbility::CanActivateAbility(UGameplayAbility* This, FGameplayAbilitySpecHandle Handle, FGameplayAbilityActorInfo* ActorInfo, FGameplayTagContainer* SourceTags, FGameplayTagContainer* TargetTags, FGameplayTagContainer* OptionalRelevantTags)
{
	bool Result = CanActivateAbilityOG(This, Handle, ActorInfo, SourceTags, TargetTags, OptionalRelevantTags);
	return Result;
}

void UGameplayAbility::Hook() {
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindUGameplayAbility_CanActivateAbility()), CanActivateAbility, (LPVOID*)&CanActivateAbilityOG);

	Log("UGameplayAbility Hooked!");
}