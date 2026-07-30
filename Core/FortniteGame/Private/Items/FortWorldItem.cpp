#include "pch.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortEnums.h"

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
	AFortPlayerController* FortPC = This->GetOwningController();
	if (!FortPC) {
		Log("UFortWorldItem::SetLoadedAmmo: GetOwningController returned null!");
		return false;
	}

	AFortInventory* Inventory = FortPC->WorldInventory;
	if (!Inventory) {
		Log("UFortWorldItem::SetLoadedAmmo: OwnerInventory is null!");
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

bool UFortWorldItem::SetPhantomReserveAmmo(UFortWorldItem* This, int32 InCount) {
	Log("UFortWorldItem::SetPhantomReserveAmmo called!");
	Log("InCount: " + std::to_string(InCount));
	return false;
}

bool UFortWorldItem::SetInInventoryOverflow(UFortWorldItem* This, bool bInInventoryOverflow) {
	//Log("UFortWorldItem::SetInInventoryOverflow called!");
	//Log("bInInventoryOverflow: " + std::to_string(bInInventoryOverflow));
	return false;
}

bool UFortWorldItem::SetDurability(UFortWorldItem* This, float InDurability) {
	AFortPlayerController* FortPC = This->GetOwningController();
	if (!FortPC) {
		//Log("UFortWorldItem::SetDurability: GetOwningController returned null!");
		return false;
	}

	AFortInventory* Inventory = FortPC->WorldInventory;
	if (!Inventory) {
		Log("UFortWorldItem::SetDurability: OwnerInventory is null!");
		return false;
	}

	FFortItemEntry* ItemEntry = Inventory->FindItemEntry(This->ItemEntry.ItemGuid);
	if (!ItemEntry) {
		Log("UFortWorldItem::SetDurability: ItemEntry is null!");
		return false;
	}

	ItemEntry->Durability = InDurability;
	Inventory->Update(ItemEntry);

	return true;
}
