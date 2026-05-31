#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayAbilitySpecHandle.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"

#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/FortGift/FortGiftingInfo.h"
#include "FortniteGame/Public/FortItem/FortItemEntryStateValue.h"

class UFortItemDefinition;
class UFortWorldItem;
class AFortInventory;
class UFortAlterationItemDefinition;
class UAthenaItemWrapDefinition;

struct FFortItemEntry : public FFastArraySerializerItem {
public:
	DefineUnrealStruct(FFortItemEntry);

	DefineStructProperty(int32, Count);
	DefineStructProperty(int32, PreviousCount);
	DefineStructProperty(UFortItemDefinition*, ItemDefinition);
	DefineStructProperty(int16, OrderIndex);
	DefineStructProperty(float, Durability);
	DefineStructProperty(int32, Level);
	DefineStructProperty(int32, LoadedAmmo);
	DefineStructProperty(int32, PhantomReserveAmmo);
	DefineStructProperty(TArray<FString>, AlterationDefinitions);
	DefineStructProperty(FString, ItemSource);
	DefineStructProperty(FGuid, ItemGuid);
	DefineStructProperty(int32, ControlOverride);
	DefineStructProperty(bool, bWasGifted);
	DefineStructProperty(bool, bIsReplicatedCopy);
	DefineStructProperty(bool, bIsDirty);
	DefineStructProperty(bool, bUpdateStatsOnCollection);
	DefineStructProperty(FFortGiftingInfo, GiftingInfo);
	DefineStructProperty(TArray<FFortItemEntryStateValue>, StateValues);
	DefineStructProperty(TWeakObjectPtr<AFortInventory>, ParentInventory);
	DefineStructProperty(FGameplayAbilitySpecHandle, GameplayAbilitySpecHandle);
	DefineStructProperty(TArray<UFortAlterationItemDefinition*>, AlterationInstances);
	DefineStructProperty(TSoftObjectPtr<UAthenaItemWrapDefinition>, WrapOverride);
	DefineStructProperty(TArray<float>, GenericAttributeValues);
public:
	uint8_t Padding[0x9C];
public:
	void SetStateValue(uint8 StateType, int32 InValue);

	void RemoveStateValue(uint8 StateType);

	int32 GetStateValue(uint8 StateType);

	void SetToDirty();

	void CopyGenericValuesFrom(const FFortItemEntry* Other);

	void SetCount(int32 NewCount);
};