#include "pch.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"

#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/Interface/FortInventoryOwnerInterface.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"

void AFortInventory::HandleInventoryLocalUpdate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleInventoryLocalUpdate");

	ProcessEvent(Func, nullptr);
}

bool AFortInventory::Update(FFortItemEntry* ItemEntry)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		return false;
	}

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

FFortItemEntry* AFortInventory::FindItemEntry(FGuid Guid)
{
	if (!this)
		return nullptr;

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

FFortItemEntry* AFortInventory::FindItemEntry(int32 ItemType)
{
	if (!Inventory.ReplicatedEntries.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;

		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemDefinition && Entry.ItemDefinition->ItemType == ItemType)
		{
			return &Entry;
		}
	}

	return nullptr;
}

FFortItemEntry* AFortInventory::FindItemEntry(UClass* ItemDefinitionClass)
{
	if (!ItemDefinitionClass)
		return nullptr;

	if (!Inventory.ReplicatedEntries.IsValid())
		return nullptr;

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;

		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemDefinition && Entry.ItemDefinition->IsA(ItemDefinitionClass))
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

TArray<FFortItemEntry*> AFortInventory::FindItemEntries(UFortItemDefinition* ItemDefinition)
{
	TArray<FFortItemEntry*> Entries;

	if (!ItemDefinition)
		return Entries;

	if (!Inventory.ReplicatedEntries.IsValid())
		return Entries;

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemDefinition == ItemDefinition)
		{
			Entries.Add(&Entry);
		}
	}

	return Entries;
}

TArray<UFortWorldItem*> AFortInventory::FindItemInstances(UFortItemDefinition* ItemDefinition)
{
	TArray<UFortWorldItem*> Items;
	if (!ItemDefinition)
		return Items;

	if (!Inventory.ItemInstances.IsValid())
		return Items;

	for (int i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		if (!Inventory.ItemInstances.IsValidIndex(i)) continue;
		auto& Item = Inventory.ItemInstances[i];
		if (Item && Item->ItemEntry.ItemDefinition == ItemDefinition)
		{
			Items.Add(Item);
		}
	}

	return Items;
}

FFortItemEntry* AFortInventory::GetCurrentItemEntry()
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	AFortPawn* Pawn = PC->Pawn->Cast<AFortPawn>();
	if (!Pawn)
	{
		return nullptr;
	}

	AFortWeapon* CurrentWeapon = Pawn->CurrentWeapon;
	if (!CurrentWeapon)
	{
		Log("AFortInventory::GetCurrentItemEntry: CurrentWeapon is null!");
		return nullptr;
	}

	return FindItemEntry(CurrentWeapon->ItemEntryGuid);
}

AFortPlayerController* AFortInventory::GetOwnerPlayerController() const
{
	return Owner ? Owner->Cast<AFortPlayerController>() : nullptr;
}

FFortItemEntry* AFortInventory::AddItem(UFortWorldItem* Item)
{
	if (!Item)
		return nullptr;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	Item->SetOwningControllerForTemporaryItem(PC);

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		FFortItemEntry& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		Entry.SetStateValue(EFortItemEntryState::GetNewItemCount(), 0);
		Update(&Entry);
	}

	InitializeExistingItem(Item);

	FGuid ItemGuid = Item->ItemEntry.ItemGuid;

	if (PC->IsUsingOldQuickBars())
	{
		PC->QuickBars->AddItemToQuickBar(ItemGuid, Item->ItemEntry.ItemDefinition->GetQuickBarForItem());
	}

	FFortItemEntry* RepEntry = FindItemEntry(ItemGuid);
	if (!RepEntry)
	{
		return nullptr;
	}

	SetStateValues(RepEntry);
	if (Update(RepEntry))
	{
		return RepEntry;
	}

	return nullptr;
}

FFortItemEntry* AFortInventory::AddItem(UFortItemDefinition* Def, int32 Count, int32 Level)
{
	if (!CanAddItem(Def, Count))
		return nullptr;

	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, Level);
	if (!Item)
	{
		Log("AFortInventory::AddItem: Failed to create temp item for " + Def->GetName().ToString());
		return nullptr;
	}

	return AddItem(Item);
}

FFortItemEntry* AFortInventory::AddItem(const FFortItemEntry& ItemEntry)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	UFortItemDefinition* Def = ItemEntry.ItemDefinition;
	if (!Def)
	{
		Log("AFortInventory::AddItem: ItemEntry.ItemDefinition is null!");
		return nullptr;
	}

	FFortItemEntry* RepEntry = AddItem(Def, ItemEntry.Count, ItemEntry.Level);
	if (!RepEntry)
		return nullptr;

	RepEntry->CopyGenericValuesFrom(&ItemEntry);

	return RepEntry;
}

int32 AFortInventory::GetOverflowFromAddingItem(const FFortItemEntry& ItemEntry)
{
	UFortItemDefinition* Def = ItemEntry.ItemDefinition;
	int32 Count = ItemEntry.Count;

	if (!CanAddItem(ItemEntry))
		return 0;

	const int32 MaxStackSize = Def->GetMaxStackSize();
	if (MaxStackSize <= 0)
		return Count;

	if (IsPrimaryItem(Def))
	{
		if (!Def->IsStackable())
		{
			while (Count > 0 && !IsInventoryFull())
			{
				FFortItemEntry NewEntry = ItemEntry;
				NewEntry.SetCount(1);

				if (!AddItem(NewEntry))
					break;

				--Count;
			}

			Update();
			return Count;
		}

		TArray<FFortItemEntry*> Entries = FindItemEntries(Def);
		for (FFortItemEntry* Entry : Entries)
		{
			if (!Entry || Count <= 0)
				break;

			Count = TryAddToEntry(*Entry, Count, MaxStackSize);
		}

		while (Count > 0 && !IsInventoryFull())
		{
			FFortItemEntry NewEntry = ItemEntry;
			NewEntry.SetCount((Count > MaxStackSize) ? MaxStackSize : Count);

			if (!AddItem(NewEntry))
				break;

			Count -= NewEntry.Count;
		}
	}
	else if (IsSecondaryItem(Def))
	{
		FFortItemEntry* Entry = FindItemEntry(Def);
		if (Entry)
		{
			Count = TryAddToEntry(*Entry, Count, MaxStackSize);
		}
		else
		{
			FFortItemEntry NewEntry = ItemEntry;
			NewEntry.SetCount((Count > MaxStackSize) ? MaxStackSize : Count);

			if (AddItem(NewEntry))
			{
				Count -= NewEntry.Count;
			}
		}
	}
	else
	{
		Log("AFortInventory::GetOverflowFromAddingItem: ItemDefinition has an invalid QuickBar type!");
		return Count;
	}

	Update();
	return Count;
}

bool AFortInventory::RemoveItem(FGuid Guid, int32 Count)
{
	if (!CanRemoveItem(Guid, Count))
		return false;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return false;

	FFortItemEntry* Entry = FindItemEntry(Guid);
	if (!Entry)
		return false;

	if (Entry->Count > Count)
	{
		Entry->Count -= Count;
		return Update(Entry);
	}

	if (PC->IsUsingOldQuickBars())
	{
		PC->QuickBars->EmptyQuickbarSlot(Guid);
	}

	RemoveEntryAndInstance(Guid);
	return Update();
}

bool AFortInventory::RemoveItem(UFortItemDefinition* Def, int32 Count)
{
	if (!Def)
	{
		Log("AFortInventory::RemoveItem: ItemDefinition is invalid!");
		return false;
	}

	if (Count <= 0)
	{
		Log("AFortInventory::RemoveItem: Count must be greater than 0!");
		return false;
	}

	FFortItemEntry* ItemEntry = FindItemEntry(Def);
	if (!ItemEntry)
	{
		Log("AFortInventory::RemoveItem: ItemEntry not found for ItemDefinition: " + Def->GetName().ToString());
		return false;
	}

	return RemoveItem(ItemEntry->ItemGuid, Count);
}

void AFortInventory::RemoveEntryAndInstance(FGuid Guid)
{
	for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemGuid == Guid)
		{
			Inventory.ReplicatedEntries.RemoveAt(i);
			break;
		}
	}

	for (int32 i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		UFortWorldItem* Item = Inventory.ItemInstances[i];
		if (Item && Item->ItemEntry.ItemGuid == Guid)
		{
			Inventory.ItemInstances.RemoveAt(i);
			break;
		}
	}
}

int32 AFortInventory::GetInventoryCapacity()
{
	if (Finder::FindAFortInventory_GetInventoryCapacity() == 0)
	{
		return 0;
	}

	if (!Owner) {
		return 0;
	}

	if (Version::Fortnite_Version >= 1.9)
	{
		IFortInventoryOwnerInterface* InventoryOwner = (IFortInventoryOwnerInterface*)Owner->GetInterfaceAddress(IFortInventoryOwnerInterface::StaticClass());
		if (!InventoryOwner)
		{
			return 0;
		}

		int32(*GetInventoryCapacityInternal)(IFortInventoryOwnerInterface*, uint8) = decltype(GetInventoryCapacityInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryCapacity());

		return GetInventoryCapacityInternal(InventoryOwner, InventoryType);
	}

	int32(*GetInventoryCapacityInternal)(AActor*, uint8) = decltype(GetInventoryCapacityInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryCapacity());
	return GetInventoryCapacityInternal(Owner, InventoryType);
}

int32 AFortInventory::GetInventoryUsed()
{
	if (Finder::FindAFortInventory_GetInventoryUsed() == 0)
	{
		return 0;
	}

	if (!Owner) {
		return 0;
	}

	if (Version::Fortnite_Version >= 1.9)
	{
		IFortInventoryOwnerInterface* InventoryOwner = (IFortInventoryOwnerInterface*)Owner->GetInterfaceAddress(IFortInventoryOwnerInterface::StaticClass());
		if (!InventoryOwner)
		{
			return 0;
		}

		int32(*GetInventoryUsedInternal)(IFortInventoryOwnerInterface*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
		return GetInventoryUsedInternal(InventoryOwner, InventoryType);
	}

	int32(*GetInventoryUsedInternal)(AActor*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
	return GetInventoryUsedInternal(Owner, InventoryType);
}

bool AFortInventory::IsInventoryFull()
{
	return GetInventoryUsed() >= GetInventoryCapacity();
}

bool AFortInventory::CanSwapForItem(UFortItemDefinition* Def)
{
	if (!IsPrimaryItem(Def))
		return false;

	if (!IsInventoryFull())
		return false;

	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry)
	{
		Log("AFortInventory::CanSwapForItem: CurrentItemEntry is null!");
		return false;
	}

	UFortItemDefinition* CurrentItemDef = CurrentItemEntry->ItemDefinition;
	if (!CurrentItemDef)
	{
		Log("AFortInventory::CanSwapForItem: CurrentItemDef is null!");
		return false;
	}

	if (!IsPrimaryItem(CurrentItemDef))
		return false;

	if (CurrentItemDef == Def && CurrentItemDef->IsStackable())
	{
		return CurrentItemEntry->Count >= CurrentItemDef->GetMaxStackSize();
	}

	return true;
}

FFortItemEntry* AFortInventory::SwapCurrentItem(const FFortItemEntry& NewItemEntry, bool bSpawnPickup)
{
	if (!CanAddItem(NewItemEntry))
		return nullptr;

	if (!CanSwapForItem(NewItemEntry.ItemDefinition))
		return nullptr;

	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry)
		return nullptr;

	const FFortItemEntry OldItemEntry = *CurrentItemEntry;
	const FGuid CurrentGuid = CurrentItemEntry->ItemGuid;
	const int32 CurrentCount = CurrentItemEntry->Count;

	if (!RemoveItem(CurrentGuid, CurrentCount))
		return nullptr;

	FFortItemEntry* AddedEntry = AddItem(NewItemEntry);
	if (!AddedEntry)
	{
		AddItem(OldItemEntry);
		return nullptr;
	}

	if (bSpawnPickup)
	{
		SpawnPickupFromEntry(OldItemEntry);
	}

	return AddedEntry;
}

bool AFortInventory::AddItemAndHandleOverflow(const FFortItemEntry& ItemEntry, bool bAllowSwap, bool bSpawnOverflowPickup)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		Log("AFortInventory::AddItemAndHandleOverflow: Owner player controller is null!");
		return false;
	}

	if (!CanAddItem(ItemEntry))
		return false;

	int32 Overflow = GetOverflowFromAddingItem(ItemEntry);
	if (Overflow <= 0)
		return true;

	FFortItemEntry OverflowEntry = ItemEntry;
	OverflowEntry.SetCount(Overflow);

	if (bAllowSwap && CanSwapForItem(OverflowEntry.ItemDefinition))
	{
		FFortItemEntry* AddedEntry = SwapCurrentItem(OverflowEntry, bSpawnOverflowPickup);
		if (AddedEntry)
		{
			PC->ServerExecuteInventoryItem(PC, AddedEntry->ItemGuid);
			if (PC->IsUsingOldQuickBars())
			{
				PC->QuickBars->EquipItem(AddedEntry->ItemGuid);
			}
			return true;
		}
	}

	if (bSpawnOverflowPickup)
	{
		SpawnPickupFromEntry(OverflowEntry);
		return true;
	}

	return false;
}

bool AFortInventory::AddItemAndHandleOverflow(UFortItemDefinition* Def, int32 Count, bool bAllowSwap, bool bSpawnOverflowPickup)
{
	if (!CanAddItem(Def, Count))
		return false;

	FFortItemEntry ItemEntry{};
	ItemEntry.ItemDefinition = Def;
	ItemEntry.SetCount(Count);
	ItemEntry.Level = 0;

	return AddItemAndHandleOverflow(ItemEntry, bAllowSwap, bSpawnOverflowPickup);
}

bool AFortInventory::IsCurrentItem(FGuid& ItemGuid)
{
	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry)
		return false;

	return CurrentItemEntry->ItemGuid == ItemGuid;
}

void AFortInventory::EquipHarvestingTool()
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return;

	FFortItemEntry* PickaxeEntry = FindItemEntry(EFortItemType::GetWeaponHarvest());
	if (!PickaxeEntry)
	{
		Log("AFortInventory::EquipHarvestingTool: HarvestingTool not found in inventory!");
		return;
	}

	PC->ServerExecuteInventoryItem(PC, PickaxeEntry->ItemGuid);
	if (PC->IsUsingOldQuickBars())
	{
		PC->QuickBars->EquipHarvestingTool();
	}
}

bool AFortInventory::DropAllItems(bool bSpawnPickups)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return false;

	TArray<FFortItemEntry> EntriesToRemove;
	for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (!Entry.ItemDefinition) {
			continue;
		}

		UFortWorldItemDefinition* ItemDef = Entry.ItemDefinition->Cast<UFortWorldItemDefinition>();
		if (!ItemDef) {
			continue;
		}
		
		if (!ItemDef->bCanBeDropped)
		{
			continue;
		}

		EntriesToRemove.Add(Entry);
	}

	for (const FFortItemEntry& Entry : EntriesToRemove)
	{
		if (bSpawnPickups)
		{
			SpawnPickupFromEntry(Entry);
		}
		RemoveItem(Entry.ItemGuid, Entry.Count);
	}

	return true;
}

bool AFortInventory::CanAddItem(UFortItemDefinition* Def, int32 Count) const
{
	return Owner && Def && Count > 0;
}

bool AFortInventory::CanAddItem(const FFortItemEntry& ItemEntry) const
{
	return Owner && ItemEntry.ItemDefinition && ItemEntry.Count > 0;
}

bool AFortInventory::CanRemoveItem(FGuid Guid, int32 Count) const
{
	if (!Guid.IsValid())
		return false;

	if (Count <= 0)
		return false;

	return GetOwnerPlayerController() != nullptr;
}

bool AFortInventory::IsPrimaryItem(UFortItemDefinition* Def) const
{
	return Def && Def->GetQuickBarForItem() == EFortQuickBars::GetPrimary();
}

bool AFortInventory::IsSecondaryItem(UFortItemDefinition* Def) const
{
	return Def && Def->GetQuickBarForItem() == EFortQuickBars::GetSecondary();
}

int32 AFortInventory::TryAddToEntry(FFortItemEntry& ItemEntry, int32 Count, int32 MaxStackSize)
{
	if (Count <= 0)
		return 0;

	const int32 Space = MaxStackSize - ItemEntry.Count;
	if (Space <= 0)
		return Count;

	const int32 ToAdd = (Count < Space) ? Count : Space;
	ItemEntry.SetCount(ItemEntry.Count + ToAdd);
	ItemEntry.bIsDirty = true;
	Inventory.MarkItemDirty(ItemEntry);

	return Count - ToAdd;
}

int32 AFortInventory::TryCreateStack(UFortItemDefinition* Def, int32 Count, int32 MaxStackSize)
{
	if (!CanAddItem(Def, Count))
		return Count;

	const int32 ToAdd = (Count > MaxStackSize) ? MaxStackSize : Count;
	return AddItem(Def, ToAdd) ? (Count - ToAdd) : Count;
}

AFortPickup* AFortInventory::SpawnPickupFromDefinition(UFortItemDefinition* Def, int32 Count)
{
	if (!CanAddItem(Def, Count))
		return nullptr;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return nullptr;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	return UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		Def,
		Count,
		PC->Pawn->K2_GetActorLocation(),
		FVector(),
		-1,
		true,
		true,
		false,
		-1,
		EFortPickupSourceTypeFlag::GetPlayer(),
		EFortPickupSpawnSource::GetUnset(),
		PC,
		false
	);
}

AFortPickup* AFortInventory::SpawnPickupFromEntry(const FFortItemEntry& ItemEntry)
{
	AFortPickup* Pickup = SpawnPickupFromDefinition(ItemEntry.ItemDefinition, ItemEntry.Count);
	Pickup->PrimaryPickupItemEntry.CopyGenericValuesFrom(&ItemEntry);
	return Pickup;
}

bool AFortInventory::SetStateValues(FFortItemEntry* ItemEntry) {
	if (!ItemEntry)
		return false;

	FGuid ItemGuid = ItemEntry->ItemGuid;

	if (UFortWorldItemDefinition* ItemDef = ItemEntry->ItemDefinition->Cast<UFortWorldItemDefinition>())
	{
		if (ItemDef->bShouldShowItemToast)
		{
			ItemEntry->SetStateValue(EFortItemEntryState::GetShouldShowItemToast(), 1);
		}
	}

	SetNewItemCountStateValue(ItemEntry, ItemEntry->Count);

	return true;
}

bool AFortInventory::SetNewItemCountStateValue(FFortItemEntry* ItemEntry, int32 Count) {
	if (!ItemEntry)
		return false;

	FGuid ItemGuid = ItemEntry->ItemGuid;

	ItemEntry->SetStateValue(EFortItemEntryState::GetNewItemCount(), Count);

	/*std::thread([this, ItemGuid]()
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		FFortItemEntry* Entry = FindItemEntry(ItemGuid);
		if (Entry)
		{
			Entry->SetStateValue(EFortItemEntryState::GetNewItemCount(), 0);
		}
	}).detach();*/

	return true;
}