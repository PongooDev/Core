#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

void AFortInventory::HandleInventoryLocalUpdate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleInventoryLocalUpdate");

	ProcessEvent(Func, nullptr);
}

FFortItemEntry* AFortInventory::FindItemEntry(FGuid Guid)
{
	if (!Guid.IsValid())
		return nullptr;
	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemGuid == Guid)
		{
			return &Entry;
		}
	}
	return nullptr;
}

FFortItemEntry* AFortInventory::FindItemEntry(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition)
		return nullptr;
	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemDefinition == ItemDefinition)
		{
			return &Entry;
		}
	}
	return nullptr;
}

UFortWorldItem* AFortInventory::FindItemInstance(FGuid Guid)
{
	if (!Guid.IsValid())
		return nullptr;
	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		auto& Item = Inventory.ItemInstances[i];
		if (Item && Item->ItemEntry.ItemGuid == Guid)
		{
			return Item;
		}
	}
	return nullptr;
}

UFortWorldItem* AFortInventory::FindItemInstance(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition)
		return nullptr;
	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		auto& Item = Inventory.ItemInstances[i];
		if (Item && Item->ItemEntry.ItemDefinition == ItemDefinition)
		{
			return Item;
		}
	}
	return nullptr;
}

FFortItemEntry* AFortInventory::AddItem(UFortItemDefinition* Def, int32 Count)
{
	if (!Def || !Owner) return nullptr;
	AFortPlayerController* PC = Owner->Cast<AFortPlayerController>();
	if (!PC) return nullptr;

	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, 0);
	if (!Item) {
		Log("Failed to create temporary item instance for item definition: " + Def->GetName().ToString());
		return nullptr;
	}

	Item->SetOwningControllerForTemporaryItem(PC);

	InitializeExistingItem(Item);

	if (Version::Fortnite_Version <= 1.8) {
		PC->QuickBars->ServerAddItemInternal(Item->ItemEntry.ItemGuid, EFortQuickBars::Max_None, -3);
	}

	return &Item->ItemEntry;
}

bool AFortInventory::Update(FFortItemEntry* ItemEntry)
{
	if (!this || !Owner) return false;

	if (ItemEntry == nullptr) {
		Inventory.MarkArrayDirty();
	}
	else {
		ItemEntry->bIsDirty = true;
		Inventory.MarkItemDirty(*ItemEntry);
	}

	bRequiresLocalUpdate = true;
	bRequiresSaving = true;

	HandleInventoryLocalUpdate();
	
	ForceNetUpdate();
	return true;
}

void AFortInventory::InitializeExistingItem(UFortWorldItem* ExistingItem) {
	void (*InitializeExistingItemInternal)(AFortInventory * This, UFortWorldItem * ExistingItem) = decltype(InitializeExistingItemInternal)(ImageBase + Finder::FindAFortInventory_InitializeExistingItem());
	return InitializeExistingItemInternal(this, ExistingItem);
}