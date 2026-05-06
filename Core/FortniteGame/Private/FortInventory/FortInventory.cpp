#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"
#include "FortniteGame/Public/Interface/FortInventoryOwnerInterface.h"

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

	if (!Inventory.ReplicatedEntries.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;

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

	if (!Inventory.ReplicatedEntries.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;

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

	if (!Inventory.ItemInstances.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		if (!Inventory.ItemInstances.IsValidIndex(i)) continue;

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

	if (!Inventory.ItemInstances.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		if (!Inventory.ItemInstances.IsValidIndex(i)) continue;

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

	return FindItemEntry(Item->ItemEntry.ItemGuid);
}

int32 AFortInventory::GetOverflowFromAddingItem(UFortItemDefinition* Def, int32 Count) {
	if (!Def || Count <= 0 || !Owner)
		return 0;

	int32 MaxStackSize = Def->GetMaxStackSize();
	FFortItemEntry* ItemEntry = FindItemEntry(Def);

	if (!ItemEntry) {
		int32 ToAdd = (Count > MaxStackSize) ? MaxStackSize : Count;
		AddItem(Def, ToAdd);
		return Count - ToAdd;
	}
	else {
		if (ItemEntry->Count >= MaxStackSize) {
			return Count;
		}
		else {
			int32 AvailableSpace = MaxStackSize - ItemEntry->Count;

			if (AvailableSpace <= 0)
			{
				return Count;
			}

			int32 ToAdd = (Count > AvailableSpace) ? AvailableSpace : Count;
			ItemEntry->Count += ToAdd;
			Update(ItemEntry);

			return Count - ToAdd;
		}
	}
}

bool AFortInventory::Update(FFortItemEntry* ItemEntry)
{
	if (!Owner) return false;

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

bool AFortInventory::RemoveItem(FGuid Guid, int32 Count) {
	if (!Guid.IsValid())
	{
		Log("AFortInventory::RemoveItem: Invalid GUID!");
		return false;
	}
	
	if (Count <= 0) {
		Log("AFortInventory::RemoveItem: Count must be greater than 0!");
		return false;
	}

	if (!Inventory.ReplicatedEntries.IsValid() || !Inventory.ItemInstances.IsValid())
	{
		Log("AFortInventory::RemoveItem: Inventory arrays are invalid!");
		return false;
	}

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i))
			continue;

		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());

		if (Entry.ItemGuid != Guid)
			continue;

		if (Entry.Count <= Count)
		{
			Inventory.ReplicatedEntries.RemoveAt(i);

			for (int j = 0; j < Inventory.ItemInstances.Num(); j++)
			{
				if (!Inventory.ItemInstances.IsValidIndex(j))
					continue;

				UFortWorldItem* Item = Inventory.ItemInstances[j];
				if (Item && Item->ItemEntry.ItemGuid == Guid)
				{
					Inventory.ItemInstances.RemoveAt(j);
					break;
				}
			}

			return Update();
		}
		else
		{
			Entry.Count -= Count;
			return Update(&Entry);
		}
	}

	Log("AFortInventory::RemoveItem: Item not found for GUID: " + Guid.FormatGuid());
	return false;
}

bool AFortInventory::RemoveItem(UFortItemDefinition* Def, int32 Count) {
	if (!Def) {
		Log("AFortInventory::RemoveItem: ItemDefinition is invalid!");
		return false;
	}

	if (Count <= 0) {
		Log("AFortInventory::RemoveItem: Count must be greater than 0!");
		return false;
	}

	FFortItemEntry* ItemEntry = FindItemEntry(Def);
	if (!ItemEntry) {
		Log("AFortInventory::RemoveItem: ItemEntry not found for ItemDefinition: " + Def->GetName().ToString());
		return false;
	}

	return RemoveItem(ItemEntry->ItemGuid, Count);
}

int32 AFortInventory::GetInventoryCapacity() {
	// This should be a vtable, atleast on some versions if not all
	if (Version::Fortnite_Version >= 1.8) {
		IFortInventoryOwnerInterface* InventoryOwner = (IFortInventoryOwnerInterface*)Owner->GetInterfaceAddress(IFortInventoryOwnerInterface::StaticClass());
		if (!InventoryOwner) {
			return 0;
		}

		int32(*GetInventoryCapacityInternal)(IFortInventoryOwnerInterface*, uint8) = decltype(GetInventoryCapacityInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryCapacity());
		return GetInventoryCapacityInternal(InventoryOwner, InventoryType);
	}
	else {
		int32(*GetInventoryCapacityInternal)(AActor*, uint8) = decltype(GetInventoryCapacityInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryCapacity());
		return GetInventoryCapacityInternal(Owner, InventoryType);
	}
}

int32 AFortInventory::GetInventoryUsed() {
	// This should be a vtable too
	if (Finder::FindAFortInventory_GetInventoryUsed() != 0) {
		if (Version::Fortnite_Version >= 1.8) {
			IFortInventoryOwnerInterface* InventoryOwner = (IFortInventoryOwnerInterface*)Owner->GetInterfaceAddress(IFortInventoryOwnerInterface::StaticClass());
			if (!InventoryOwner) {
				return 0;
			}

			int32(*GetInventoryUsedInternal)(IFortInventoryOwnerInterface*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
			return GetInventoryUsedInternal(InventoryOwner, InventoryType);
		}
		else {
			int32(*GetInventoryUsedInternal)(AActor*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
			return GetInventoryUsedInternal(Owner, InventoryType);
		}
	}
	else {
		return 0;
	}
}

bool AFortInventory::IsInventoryFull() {
	int32 Capacity = GetInventoryCapacity();
	int32 Used = GetInventoryUsed();

	return Used >= Capacity;
}