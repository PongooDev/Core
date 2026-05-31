#include "pch.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"

#include "FortniteGame/Public/FortItem/FortItemEntryStateValue.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

void FFortItemEntry::SetStateValue(uint8 StateType, int32 InValue) {
	for (int32 i = 0; i < StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateType) {
			StateValue.IntValue = InValue;
			return;
		}
	}

	FFortItemEntryStateValue StateValue{};

	StateValue.IntValue = InValue;
	StateValue.StateType = StateType;

	StateValues.Add(StateValue, FFortItemEntryStateValue::GetSize());
}

void FFortItemEntry::RemoveStateValue(uint8 StateType) {
	for (int32 i = 0; i < StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateType) {
			StateValues.RemoveAt(i);
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
	if (NewCount > Count) {
		int32 CountDifference = NewCount - Count;

		AFortInventory* Inventory = ParentInventory.Get();
		if (Inventory) {
			Inventory->SetNewItemCountStateValue(this, CountDifference);
		}
	}

	Count = NewCount;
	SetToDirty();
}