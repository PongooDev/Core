#include "pch.h"
#include "../Public/AbilitySystemComponent.h"

#include "../Public/Abilities/GameplayAbility.h"

void UAbilitySystemComponent::ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) {
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ClientActivateAbilityFailed"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ClientActivateAbilityFailedInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, int16) = decltype(ClientActivateAbilityFailedInternal)(VTable[VTableIdx]);
		ClientActivateAbilityFailedInternal(this, AbilityToActivate, PredictionKey);
	}
}

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
{
	FGameplayAbilitySpec* (*FindAbilitySpecFromHandleInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle) = decltype(FindAbilitySpecFromHandleInternal)(ImageBase + Finder::FindUAbilitySystemComponent_FindAbilitySpecFromHandle());
	return FindAbilitySpecFromHandleInternal(this, Handle);
}

void UAbilitySystemComponent::InternalServerTryActivateAbility(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) {
	FGameplayAbilitySpec* Spec = This->FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	This->ConsumeAllReplicatedData(Handle, PredictionKey);

	const UGameplayAbility* AbilityToActivate = Spec->Ability;
	if (!AbilityToActivate)
	{
		This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	UGameplayAbility* InstancedAbility = nullptr;
	Spec->InputPressed = true;

	if (This->InternalTryActivateAbility(Handle, PredictionKey, &InstancedAbility, nullptr, TriggerEventData))
	{

	}
	else
	{
		Log("InternalServerTryActiveAbility. Rejecting ClientActivation of " + Spec->Ability->GetName().ToString() + ". InternalTryActivateAbility failed.");
		This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		Spec->InputPressed = false;

		if (Version::Engine_Version >= 4.22) {
			This->ActivatableAbilities.MarkItemDirty(*Spec);
		}
	}
	if (Version::Engine_Version <= 4.21) {
		This->ActivatableAbilities.MarkItemDirty(*Spec);
	}
}

void UAbilitySystemComponent::ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey)
{
	// its not stripped but its super hard to find, if anyone wants to find it be my guest!!

	uintptr_t ActivatableAbilitiesOffset = StaticClass()->GetPropertyOffset("ActivatableAbilities");
	uintptr_t AbilityTargetDataMapOffset = __int64(this) + ActivatableAbilitiesOffset + FGameplayAbilitySpecContainer::GetSize();

	if (Version::Engine_Version >= 4.22) {
		FGameplayAbilityReplicatedDataContainer& AbilityTargetDataMap = *(FGameplayAbilityReplicatedDataContainer*)(AbilityTargetDataMapOffset);

		TSharedPtr<FAbilityReplicatedDataCache> CachedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, AbilityOriginalPredictionKey));
		if (CachedData.IsValid())
		{
			CachedData->Reset();
		}
	}
	else {
		TMap<FGameplayAbilitySpecHandleAndPredictionKey, FAbilityReplicatedDataCache>& AbilityTargetDataMap = *(TMap<FGameplayAbilitySpecHandleAndPredictionKey, FAbilityReplicatedDataCache>*)(AbilityTargetDataMapOffset);

		FAbilityReplicatedDataCache* CachedData = AbilityTargetDataMap.Find(FGameplayAbilitySpecHandleAndPredictionKey(AbilityHandle, AbilityOriginalPredictionKey));
		if (CachedData)
		{
			CachedData->Reset();
		}
	}
}

bool UAbilitySystemComponent::InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, const FGameplayEventData* TriggerEventData)
{
	bool (*InternalTryActivateAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, FPredictionKey, UGameplayAbility**, void*, const FGameplayEventData*) = decltype(InternalTryActivateAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_InternalTryActivateAbility());
	return InternalTryActivateAbilityInternal(this, AbilityToActivate, InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbility(FGameplayAbilitySpec& AbilitySpec) {
	static void (*GiveAbilityInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec) = decltype(GiveAbilityInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbility());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityInternal(this, &OutHandle, AbilitySpec);
	return OutHandle;
}

FGameplayAbilitySpecHandle UAbilitySystemComponent::GiveAbilityAndActivateOnce(FGameplayAbilitySpec* Spec, FGameplayEventData* GameplayEventData)
{
	static void (*GiveAbilityAndActivateOnceInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle*, FGameplayAbilitySpec*, FGameplayEventData*) = decltype(GiveAbilityAndActivateOnceInternal)(ImageBase + Finder::FindUAbilitySystemComponent_GiveAbilityAndActivateOnce());
	FGameplayAbilitySpecHandle OutHandle;
	GiveAbilityAndActivateOnceInternal(this, &OutHandle, Spec, GameplayEventData);
	return OutHandle;
}