#include "pch.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"

#include "FortniteGame/Public/FortItem/FortItemEntryStateValue.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

void FFortItemEntry::SetStateValue(EFortItemEntryState StateType, int32 InValue) {
	for (int32 i = 0; i < StateValues.Num(); i++) {
		FFortItemEntryStateValue& StateValue = StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());

		if (StateValue.StateType == StateType) {
			StateValue.IntValue = InValue;
			return;
		}
	}

	FFortItemEntryStateValue StateValue;

	StateValue.StateType = StateType;
	StateValue.IntValue = InValue;

	StateValues.Add(StateValue);
}

void FFortItemEntry::SetToDirty() {
	if (ParentInventory.Get()) {
		ParentInventory.Get()->Update(this);
	}
	bIsDirty = true;
}