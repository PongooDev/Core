#include "pch.h"
#include "../Public/AbilitySystemComponent.h"

#include "../Public/Abilities/GameplayAbility.h"

void UAbilitySystemComponent::ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) {
	void (*ClientActivateAbilityFailedInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, int16) = decltype(ClientActivateAbilityFailedInternal)(ImageBase + Finder::FindUAbilitySystemComponent_ClientActivateAbilityFailed());
	ClientActivateAbilityFailedInternal(this, AbilityToActivate, PredictionKey);
}

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
{
	// Check if UE 5 versions are different
	for (int i = 0; i < ActivatableAbilities.Items.Num(); i++)
	{
		FGameplayAbilitySpec& Spec = ActivatableAbilities.Items.GetWithSize(i, FGameplayAbilitySpec::GetSize());
		if (Spec.Handle == Handle)
		{
			return &Spec;
		}
	}

	return nullptr;
}

void UAbilitySystemComponent::InternalServerTryActivateAbility(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) {
	if (Version::Engine_Version == 4.16) {
		FGameplayAbilitySpec* Spec = This->FindAbilitySpecFromHandle(Handle);
		if (!Spec)
		{
			// Can potentially happen in race conditions where client tries to activate ability that is removed server side before it is received.
			This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
			return;
		}

		// Consume any pending target info, to clear out cancels from old executions
		This->ConsumeAllReplicatedData(Handle, PredictionKey);

		const UGameplayAbility* AbilityToActivate = Spec->Ability;
		if (!AbilityToActivate)
		{
			return;
		}

		UGameplayAbility* InstancedAbility = nullptr;
		Spec->InputPressed = true;

		// Attempt to activate the ability (server side) and tell the client if it succeeded or failed.
		if (This->InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
		{
			// TryActivateAbility handles notifying the client of success
		}
		else
		{
			Log("InternalServerTryActiveAbility. Rejecting ClientActivation of " + Spec->Ability->GetName().ToString() + ". InternalTryActivateAbility failed.");
			This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
			Spec->InputPressed = false;
		}
		This->ActivatableAbilities.MarkItemDirty(*Spec);
	}
}

void UAbilitySystemComponent::ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey)
{
	// This is not stripped so i can find the offset at some point
}

bool UAbilitySystemComponent::InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	bool (*InternalTryActivateAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UGameplayAbility**, void*, const FGameplayEventData*) = decltype(InternalTryActivateAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_InternalTryActivateAbility());
	return InternalTryActivateAbilityInternal(this, AbilityToActivate, InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

FGameplayAbilitySpec UAbilitySystemComponent::BuildAbilitySpecFromClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level /*= 0*/, int32 InputID /*= -1*/)
{
	// validate the class
	if (!AbilityClass)
	{
		Log("BuildAbilitySpecFromClass called with an invalid Ability Class.");

		return FGameplayAbilitySpec();
	}

	// get the CDO. GiveAbility will validate so we don't need to
	UGameplayAbility* AbilityCDO = (UGameplayAbility*)AbilityClass->GetDefaultObject();

	// build the ability spec
	// we need to initialize this through the constructor,
	// or the Handle won't be properly set and will cause errors further down the line
	return FGameplayAbilitySpec(AbilityCDO, Level, InputID);
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::K2_GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level, int32 InputID) {
	if (Version::Engine_Version >= 5.00) {
		// Call K2_GiveAbilityInternal
	}
	else {
		// Recreate impl for ue 5.0 to be used on lower versions as a fallback

		// build and validate the ability spec
		FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(AbilityClass, Level, InputID);

		// validate the class
		if (!AbilitySpec.Ability)
		{
			Log("K2_GiveAbility() called with an invalid Ability Class.");

			return FGameplayAbilitySpecHandle();
		}

		// grant the ability and return the handle. This will run validation and authority checks
		return GiveAbility(AbilitySpec);
	}
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbility(FGameplayAbilitySpec& AbilitySpec) {
	static void (*GiveAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbility());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityInternal(this, &OutHandle, AbilitySpec);
	return OutHandle;
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbilityAndActivateOnce(FGameplayAbilitySpec& Spec, FGameplayEventData* GameplayEventData)
{
	static void (*GiveAbilityAndActivateOnceInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec, FGameplayEventData*) = decltype(GiveAbilityAndActivateOnceInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbilityAndActivateOnce());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityAndActivateOnceInternal(this, &OutHandle, Spec, GameplayEventData);
	return OutHandle;
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::K2_GiveAbilityAndActivateOnce(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level /* = 0*/, int32 InputID /* = -1*/)
{
	// build and validate the ability spec
	FGameplayAbilitySpec AbilitySpec = BuildAbilitySpecFromClass(AbilityClass, Level, InputID);

	// validate the class
	if (!AbilitySpec.Ability)
	{
		Log("K2_GiveAbilityAndActivateOnce() called with an invalid Ability Class.");

		return FGameplayAbilitySpecHandle();
	}

	return GiveAbilityAndActivateOnce(AbilitySpec);
}