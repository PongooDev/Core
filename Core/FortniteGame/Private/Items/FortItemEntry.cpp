#include "pch.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"

#include "FortniteGame/Public/Items/FortItemEntryStateValue.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"

void FFortItemEntry::SetStateValue(uint8 StateType, int32 InValue) {
	for (int32 i = 0; i < StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateType) {
			StateValue.IntValue = InValue;
			return;
		}
	}

	FFortItemEntryStateValue StateValue = *FFortItemEntryStateValue::Allocate();

	StateValue.IntValue = InValue;
	StateValue.StateType = StateType;

	StateValues.Add(StateValue, FFortItemEntryStateValue::GetSize());
}

void FFortItemEntry::RemoveStateValue(uint8 StateType) {
	for (int32 i = StateValues.Num() - 1; i >= 0; i--) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateType) {
			StateValues.RemoveAt(i, FFortItemEntryStateValue::GetSize());
		}
	}
}

int32 FFortItemEntry::GetStateValue(uint8 StateType) {
	for (int32 i = 0; i < StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateType) {
			return StateValue.IntValue;
		}
	}

	return 0;
}

void FFortItemEntry::SetToDirty() {
	bIsDirty = true;
}

void FFortItemEntry::CopyGenericValuesFrom(const FFortItemEntry* Other) {
	bUpdateStatsOnCollection = Other->bUpdateStatsOnCollection;
	ControlOverride = Other->ControlOverride;
	Durability = Other->Durability;
	GiftingInfo = Other->GiftingInfo;
	ItemSource = Other->ItemSource;
	LoadedAmmo = Other->LoadedAmmo;
	PhantomReserveAmmo = Other->PhantomReserveAmmo;

	SetToDirty();
}

void FFortItemEntry::SetCount(int32 NewCount) {
	AFortInventory* Inventory = ParentInventory.Get();

	if (NewCount > Count) {
		int32 CountDifference = NewCount - Count;

		if (Inventory) {
			Inventory->SetNewItemCountStateValue(this, CountDifference);
		}
	}

	Count = NewCount;
	if (Inventory) {
		Inventory->Update(this);
	}
	else {
		SetToDirty();
	}
}