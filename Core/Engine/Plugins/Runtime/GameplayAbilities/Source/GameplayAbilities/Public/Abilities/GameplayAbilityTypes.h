#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "GameplayAbilityTargetTypes.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpecHandle.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayPrediction.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

class APlayerController;
class UAbilitySystemComponent;
class UAnimMontage;
class UGameplayAbility;
class UMovementComponent;
class UDataTable;
class UGameplayTask;
struct FAbilityTargetDataSetDelegate;
struct FSimpleMulticastDelegate;
struct FAbilityReplicatedData;

struct FGameplayEventData {
public:
	DefineUnrealStruct(FGameplayEventData);
};

struct FGameplayAbilitySpecHandleAndPredictionKey {
public:
	DefineUnrealStruct(FGameplayAbilitySpecHandleAndPredictionKey);

	DefineStructProperty(FGameplayAbilitySpecHandle, AbilityHandle);
	DefineStructProperty(int32, PredictionKeyAtCreation);
public:
	FGameplayAbilitySpecHandleAndPredictionKey()
	{
		PredictionKeyAtCreation = 0;
	}

	FGameplayAbilitySpecHandleAndPredictionKey(const FGameplayAbilitySpecHandle& HandleRef, const FPredictionKey& PredictionKeyAtCreationRef)
	{
		AbilityHandle = HandleRef;
		PredictionKeyAtCreation = PredictionKeyAtCreationRef.Current;
	}

	bool operator==(const FGameplayAbilitySpecHandleAndPredictionKey& Other) const
	{
		return AbilityHandle == Other.AbilityHandle && PredictionKeyAtCreation == Other.PredictionKeyAtCreation;
	}

	bool operator!=(const FGameplayAbilitySpecHandleAndPredictionKey& Other) const
	{
		return AbilityHandle != Other.AbilityHandle || PredictionKeyAtCreation != Other.PredictionKeyAtCreation;
	}
};

struct FAbilityReplicatedDataCache {
public:
	void** VTable;

	FGameplayAbilityTargetDataHandle& GetTargetData() {
		return *reinterpret_cast<FGameplayAbilityTargetDataHandle*>(reinterpret_cast<uintptr_t>(this) + 0x8);
	}

	FGameplayTag& GetApplicationTag() {
		return *reinterpret_cast<FGameplayTag*>(
			reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize());
	}

	bool& GetbTargetConfirmed() {
		return *reinterpret_cast<bool*>(
			reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize()
			+ FGameplayTag::GetSize());
	}

	bool& GetbTargetCancelled() {
		return *reinterpret_cast<bool*>(
			reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize()
			+ FGameplayTag::GetSize()
			+ sizeof(bool));
	}

	FAbilityTargetDataSetDelegate& GetTargetSetDelegate() {
		return *reinterpret_cast<FAbilityTargetDataSetDelegate*>(
			reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize()
			+ FGameplayTag::GetSize()
			+ 0x8);
	}

	FSimpleMulticastDelegate& GetTargetCancelledDelegate() {
		return *reinterpret_cast<FSimpleMulticastDelegate*>(
			reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize()
			+ FGameplayTag::GetSize()
			+ 0x8
			+ 0x10);
	}

	uintptr_t GetGenericEventsBase() {
		return reinterpret_cast<uintptr_t>(this) + 0x8
			+ FGameplayAbilityTargetDataHandle::GetSize()
			+ FGameplayTag::GetSize()
			+ 0x8
			+ 0x10   // TargetSetDelegate
			+ 0x10;  // TargetCancelledDelegate
	}

	FAbilityReplicatedData& GetGenericEvent(int32 Index) {
		return *reinterpret_cast<FAbilityReplicatedData*>(
			GetGenericEventsBase() + (sizeof(FAbilityReplicatedData) * Index));
	}

	FPredictionKey& GetPredictionKey() {
		return *reinterpret_cast<FPredictionKey*>(
			GetGenericEventsBase()
			+ (sizeof(FAbilityReplicatedData) * (int32)EAbilityGenericReplicatedEvent::MAX));
	}

	/** Resets any cached data, leaves delegates up */
	void Reset()
	{
		GetbTargetConfirmed() = GetbTargetCancelled() = false;
		GetTargetData() = FGameplayAbilityTargetDataHandle();
		GetApplicationTag() = FGameplayTag();
		GetPredictionKey() = FPredictionKey();
		for (int32 i = 0; i < (int32)EAbilityGenericReplicatedEvent::MAX; ++i)
		{
			GetGenericEvent(i).bTriggered = false;
			GetGenericEvent(i).VectorPayload = FVector(0, 0, 0);
		}
	}
};

struct FGameplayAbilityReplicatedDataContainer
{
	TSharedPtr<FAbilityReplicatedDataCache> Find(const FGameplayAbilitySpecHandleAndPredictionKey& Key) const;

private:

	typedef TPair<FGameplayAbilitySpecHandleAndPredictionKey, TSharedRef<FAbilityReplicatedDataCache>> FKeyDataPair;

	TArray<FKeyDataPair> InUseData;
	TArray<TSharedRef<FAbilityReplicatedDataCache>> FreeData;
};