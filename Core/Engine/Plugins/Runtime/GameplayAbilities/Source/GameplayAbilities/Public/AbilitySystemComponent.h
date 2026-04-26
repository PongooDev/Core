#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTasks/Classes/GameplayTasksComponent.h"
#include "GameplayPrediction.h"
#include "Abilities/GameplayAbilityTypes.h"
#include "GameplayAbilitySpecHandle.h"
#include "GameplayAbilitySpec.h"

class AGameplayAbilityTargetActor;
class AHUD;
class FDebugDisplayInfo;
class UAnimMontage;
class UCanvas;
class UInputComponent;

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
public:
	void ClientActivateAbilityFailed(FGameplayAbilitySpecHandle AbilityToActivate, int16 PredictionKey);

	FGameplayAbilitySpec* FindAbilitySpecFromHandle(FGameplayAbilitySpecHandle Handle);

	static void InternalServerTryActivateAbility(UAbilitySystemComponent* This, FGameplayAbilitySpecHandle AbilityToActivate, bool InputPressed, const FPredictionKey& PredictionKey, FGameplayEventData* TriggerEventData);

	void ConsumeAllReplicatedData(FGameplayAbilitySpecHandle AbilityHandle, FPredictionKey AbilityOriginalPredictionKey);

	bool InternalTryActivateAbility(FGameplayAbilitySpecHandle AbilityToActivate, FPredictionKey InPredictionKey = FPredictionKey(), UGameplayAbility** OutInstancedAbility = nullptr, void* OnGameplayAbilityEndedDelegate = nullptr, const FGameplayEventData* TriggerEventData = nullptr);

	FGameplayAbilitySpec BuildAbilitySpecFromClass(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0, int32 InputID = -1);

	FGameplayAbilitySpecHandle K2_GiveAbility(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0, int32 InputID = -1);

	FGameplayAbilitySpecHandle GiveAbility(FGameplayAbilitySpec& AbilitySpec);

	FGameplayAbilitySpecHandle GiveAbilityAndActivateOnce(FGameplayAbilitySpec& AbilitySpec, FGameplayEventData* GameplayEventData = nullptr);

	FGameplayAbilitySpecHandle K2_GiveAbilityAndActivateOnce(TSubclassOf<UGameplayAbility> AbilityClass, int32 Level = 0, int32 InputID = -1);
public:
	static void Hook() {
		HookEveryVTableIdx(UAbilitySystemComponent::StaticClass(), Finder::FindInternalServerTryActivateAbilityVFT(), InternalServerTryActivateAbility);

		Log("UAbilitySystemComponent Hooked!");
	}
};