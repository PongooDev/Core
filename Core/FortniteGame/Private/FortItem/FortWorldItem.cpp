#include "pch.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"

#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

uint8 UFortWorldItem::GetQuickBarForItem() {
	if (!ItemEntry.ItemDefinition) {
		Log("UFortWorldItem::GetQuickBarForItem: ItemEntry.ItemDefinition is null!");
		return EFortQuickBars::GetMax_None();
	}

	return ItemEntry.ItemDefinition->GetQuickBarForItem();
}

void UFortWorldItem::SetOwningInventory(AFortInventory* NewOwnerInventory) {
	void (*&SetOwningInventoryInternal)(UFortWorldItem* This, AFortInventory* NewOwnerInventory) = decltype(SetOwningInventoryInternal)(VTable[Finder::FindUFortWorldItem_SetOwningInventoryVFT()]);
	SetOwningInventoryInternal(this, NewOwnerInventory);
}

bool UFortWorldItem::SetLoadedAmmo(UFortWorldItem* This, int32 InCount) {
	AFortPlayerController* PC = This->GetOwningController();
	if (!PC) {
		Log("UFortWorldItem::SetLoadedAmmo: OwningController is null!");
		return false;
	}

	AFortInventory* Inventory = PC->WorldInventory;
	if (!Inventory) {
		Log("UFortWorldItem::SetLoadedAmmo: WorldInventory is null!");
		return false;
	}

	FFortItemEntry* ItemEntry = Inventory->FindItemEntry(This->ItemEntry.ItemGuid);
	if (!ItemEntry) {
		Log("UFortWorldItem::SetLoadedAmmo: ItemEntry is null!");
		return false;
	}

	ItemEntry->LoadedAmmo = InCount;
	Inventory->Update(ItemEntry);

	return true;
}