#include "pch.h"
#include "../Public/AbilitySystemComponent.h"

#include "../Public/Abilities/GameplayAbility.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

void UAbilitySystemComponent::ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey) {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientActivateAbilityFailed");

	if (!Func) {
		return;
	}

	Call(Func, AbilityToActivate, PredictionKey);
}

FGameplayAbilitySpec* UAbilitySystemComponent::FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle)
{
	FGameplayAbilitySpec* (*FindAbilitySpecFromHandleInternal)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle) = decltype(FindAbilitySpecFromHandleInternal)(ImageBase + Finder::FindUAbilitySystemComponent_FindAbilitySpecFromHandle());
	return FindAbilitySpecFromHandleInternal(this, Handle);
}

void UAbilitySystemComponent::InternalServerTryActivateAbility(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle Handle, bool InputPressed, const FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData) {
	//Log("UAbilitySystemComponent::InternalServerTryActivateAbility");
	FGameplayAbilitySpec* Spec = This->FindAbilitySpecFromHandle(Handle);
	if (!Spec)
	{
		Log("InternalServerTryActiveAbility. Rejecting ClientActivation of ability with invalid SpecHandle!");
		This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	const UGameplayAbility* AbilityToActivate = Spec->Ability;
	if (!AbilityToActivate)
	{
		Log("InternalServerTryActiveAbility. Rejecting ClientActivation of unconfigured spec ability!");
		This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
		return;
	}

	if (AbilityToActivate->_HasInstancingPolicy()) {
		if (AbilityToActivate->NetSecurityPolicy == EGameplayAbilityNetSecurityPolicy::GetServerOnlyExecution() ||
			AbilityToActivate->NetSecurityPolicy == EGameplayAbilityNetSecurityPolicy::GetServerOnly())
		{
			Log("InternalServerTryActiveAbility. Rejecting ClientActivation of " + AbilityToActivate->GetName().ToString() + " due to security policy violation.");
			This->ClientActivateAbilityFailed(Handle, PredictionKey.Current);
			return;
		}
	}

	This->ConsumeAllReplicatedData(Handle, PredictionKey);

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

bool UAbilitySystemComponent::InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, FGameplayEventData* TriggerEventData)
{
	const uintptr_t Addr = ImageBase + Finder::FindUAbilitySystemComponent_InternalTryActivateAbility();
	if (FPredictionKey::GetSize() == 0x18)
	{
		using Fn = bool(*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, _Pad_0x18, UGameplayAbility**, void*, void*);
		return ((Fn)Addr)(this, AbilityToActivate, *(_Pad_0x18*)&InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
	}
	else
	{
		using Fn = bool(*)(UAbilitySystemComponent*, FGameplayAbilitySpecHandle, _Pad_0x10, UGameplayAbility**, void*, void*);
		return ((Fn)Addr)(this, AbilityToActivate, *(_Pad_0x10*)&InPredictionKey, OutInstancedAbility, OnGameplayAbilityEndedDelegate, TriggerEventData);
	}
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

FActiveGameplayEffectHandle UAbilitySystemComponent::BP_ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, const FGameplayEffectContextHandle& EffectContext)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("BP_ApplyGameplayEffectToSelf");

	if (!Func) {
		return FActiveGameplayEffectHandle();
	}

	return Call<FActiveGameplayEffectHandle>(Func, GameplayEffectClass, Level, EffectContext);
}

FGameplayEffectContextHandle UAbilitySystemComponent::MakeEffectContext() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("MakeEffectContext");

	if (!Func) {
		return FGameplayEffectContextHandle();
	}

	return const_cast<UAbilitySystemComponent*>(this)->Call<FGameplayEffectContextHandle>(Func);
}

void UAbilitySystemComponent::ClientCancelAbility(FGameplayAbilitySpecHandle& AbilityToCancel, FGameplayAbilityActivationInfo& ActivationInfo)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientCancelAbility");

	if (!Func) {
		return;
	}

	Call(Func, AbilityToCancel, ActivationInfo);
}

void UAbilitySystemComponent::ClientEndAbility(FGameplayAbilitySpecHandle& AbilityToEnd, FGameplayAbilityActivationInfo& ActivationInfo)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientEndAbility");

	if (!Func) {
		return;
	}

	Call(Func, AbilityToEnd, ActivationInfo);
}

void UAbilitySystemComponent::ServerEndAbility(FGameplayAbilitySpecHandle& AbilityToEnd, FGameplayAbilityActivationInfo& ActivationInfo, const FPredictionKey& PredictionKey)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerEndAbility");

	if (!Func) {
		return;
	}

	Call(Func, AbilityToEnd, ActivationInfo, PredictionKey);
}