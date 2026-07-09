#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTasks/Classes/GameplayTasksComponent.h"
#include "GameplayPrediction.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayAbilitySpec.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayEffectTypes.h"

class AGameplayAbilityTargetActor;
class AHUD;
class FDebugDisplayInfo;
class UAnimMontage;
class UCanvas;
class UInputComponent;
class UGameplayEffect;

enum class EReplicationMode : uint8
{
	/** Only replicate minimal gameplay effect info*/
	Minimal,
	/** Only replicate minimal gameplay effect info to simulated proxies but full info to owners and autonomous proxies */
	Mixed,
	/** Replicate full gameplay info to all */
	Full,
};

enum class EGameplayEffectReplicationMode : uint8
{
	/** Only replicate minimal gameplay effect info. Note: this does not work for Owned AbilitySystemComponents (Use Mixed instead). */
	Minimal,
	/** Only replicate minimal gameplay effect info to simulated proxies but full info to owners and autonomous proxies */
	Mixed,
	/** Replicate full gameplay info to all */
	Full,
};

/** When performing actions (such as gathering activatable abilities), how do we deal with Pending items (e.g. abilities not yet added or removed) */
enum class EConsiderPending : uint8
{
	/** Don't consider any Pending actions (such as Pending Abilities Added or Removed) */
	None = 0,

	/** Consider Pending Adds when performing the action */
	PendingAdd = (1 << 0),

	/** Consider Pending Removes when performing the action */
	PendingRemove = (1 << 1),

	All = PendingAdd | PendingRemove
};

class UAbilitySystemComponent : public UGameplayTasksComponent {
public:
	DefineUnrealClass(UAbilitySystemComponent);

	DefineUProperty(FGameplayAbilitySpecContainer, ActivatableAbilities);
	DefineUProperty(AActor*, AvatarActor);
public:
	void ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey);

	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle);

	static void InternalServerTryActivateAbility(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, const FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData);

	void ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey);

	bool InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey, UGameplayAbility** OutInstancedAbility, void* OnGameplayAbilityEndedDelegate, FGameplayEventData* TriggerEventData);

	FGameplayAbilitySpecHandle GiveAbility(FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpecHandle GiveAbilityAndActivateOnce(FGameplayAbilitySpec* AbilitySpec, FGameplayEventData* GameplayEventData = nullptr);

	FActiveGameplayEffectHandle BP_ApplyGameplayEffectToSelf(TSubclassOf<UGameplayEffect> GameplayEffectClass, float Level, const FGameplayEffectContextHandle& EffectContext);

	FGameplayEffectContextHandle MakeEffectContext() const;

	TSharedPtr<FGameplayAbilityActorInfo>& GetAbilityActorInfo() {
		uintptr_t AvatarActorOffset = StaticClass()->GetPropertyOffset("AvatarActor");
		return *(TSharedPtr<FGameplayAbilityActorInfo>*)(__int64(this) + AvatarActorOffset + 8);
	}

	void ClientCancelAbility(FGameplayAbilitySpecHandle& AbilityToCancel, FGameplayAbilityActivationInfo& ActivationInfo);

	void ClientEndAbility(FGameplayAbilitySpecHandle& AbilityToEnd, FGameplayAbilityActivationInfo& ActivationInfo);

	void ServerEndAbility(FGameplayAbilitySpecHandle& AbilityToEnd, FGameplayAbilityActivationInfo& ActivationInfo, const FPredictionKey& PredictionKey);
public:
	static void Hook() {
		HookEveryVTableIdx(UAbilitySystemComponent::StaticClass(), Finder::FindInternalServerTryActivateAbilityVFT(), InternalServerTryActivateAbility);

		Log("UAbilitySystemComponent Hooked!");
	}
};