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
	if (!this)
		return;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleInventoryLocalUpdate");

	if (!Func) {
		Log("AFortInventory::HandleInventoryLocalUpdate: Function not found!");
		return;
	}

	Call(Func);
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
	if (Finder::FindAFortInventory_InitializeExistingItem()) {
		void (*InitializeExistingItemInternal)(AFortInventory * This, UFortWorldItem * ExistingItem) = decltype(InitializeExistingItemInternal)(ImageBase + Finder::FindAFortInventory_InitializeExistingItem());
		return InitializeExistingItemInternal(this, ExistingItem);
	}
	else {
		Inventory.ReplicatedEntries.Add(ExistingItem->ItemEntry);
		Inventory.ItemInstances.Add(ExistingItem);
	}
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

FFortItemEntry* AFortInventory::FindItemEntry(UFortItemDefinition* ItemDefinition, bool bFindFirstNotFull)
{
	if (!ItemDefinition)
		return nullptr;

	if (!Inventory.ReplicatedEntries.IsValid())
		return nullptr;

	int32 MaxStackSize = ItemDefinition->GetMaxStackSize();

	for (int i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		if (!Inventory.ReplicatedEntries.IsValidIndex(i)) continue;

		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemDefinition == ItemDefinition && (!bFindFirstNotFull || Entry.Count < MaxStackSize))
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

FFortItemEntry* AFortInventory::AddItem(UFortWorldItem* Item, bool bDeferUpdate)
{
	if (!Item)
		return nullptr;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	Item->SetOwningControllerForTemporaryItem(PC);
	SetStateValues(&Item->ItemEntry);

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
	if (bDeferUpdate ? true : Update(RepEntry))
	{
		return RepEntry;
	}

	return nullptr;
}

FFortItemEntry* AFortInventory::AddItem(UFortItemDefinition* Def, int32 Count, int32 Level, bool bDeferUpdate)
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

FFortItemEntry* AFortInventory::AddItem(const FFortItemEntry& ItemEntry, bool bDeferUpdate)
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

	RepEntry->LoadedAmmo = ItemEntry.LoadedAmmo;
	RepEntry->Durability = ItemEntry.Durability;
	RepEntry->bIsDirty = true;

	if (bDeferUpdate ? true : Update(RepEntry))
	{
		return RepEntry;
	}

	return nullptr;
}

FFortItemEntry* AFortInventory::AddItemPreserveGuid(const FFortItemEntry& ItemEntry)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	UFortItemDefinition* Def = ItemEntry.ItemDefinition;
	if (!Def)
		return nullptr;

	if (FindItemEntry(ItemEntry.ItemGuid))
	{
		Log("AddItemPreserveGuid: guid already exists in inventory!");
		return nullptr;
	}

	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(ItemEntry.Count, ItemEntry.Level);
	if (!Item)
		return nullptr;

	FFortItemEntry::Copy(&Item->ItemEntry, &ItemEntry);

	Item->SetOwningControllerForTemporaryItem(PC);
	SetStateValues(&Item->ItemEntry);

	InitializeExistingItem(Item);

	if (PC->IsUsingOldQuickBars())
		PC->QuickBars->AddItemToQuickBar(ItemEntry.ItemGuid, Def->GetQuickBarForItem());

	FFortItemEntry* RepEntry = FindItemEntry(ItemEntry.ItemGuid);
	if (!RepEntry)
		return nullptr;

	if (Update(RepEntry))
		return RepEntry;

	return nullptr;
}

int32 AFortInventory::GetOverflowFromAddingItem(const FFortItemEntry& ItemEntry)
{
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return ItemEntry.Count;

	UFortItemDefinition* Def = ItemEntry.ItemDefinition;
	int32 Remaining = ItemEntry.Count;

	if (!Def || Remaining <= 0)
		return 0;

	const int32 MaxStackSize = Def->GetMaxStackSize();
	if (MaxStackSize <= 0)
		return Remaining;

	FGuid FocusedGuid;
	const bool bHasFocusedGuid =
		PC->MyFortPawn &&
		PC->MyFortPawn->CurrentWeapon &&
		(FocusedGuid = PC->MyFortPawn->CurrentWeapon->ItemEntryGuid, true); // setting the FocusedGuid on this line

	bool bChanged = false;

	if (IsPrimaryItem(Def))
	{
		if (!Def->IsStackable())
		{
			while (Remaining > 0 && !IsInventoryFull())
			{
				FFortItemEntry NewEntry = ItemEntry;
				NewEntry.SetCount(1);

				if (!AddItem(NewEntry))
					break;

				--Remaining;
				bChanged = true;
			}

			if (bChanged) Update();
			return Remaining;
		}

		// Stacking logic begins here

		// 1) Fill focused matching stack first (the item the player is currently holding)
		if (bHasFocusedGuid)
		{
			if (FFortItemEntry* FocusedEntry = FindItemEntry(FocusedGuid))
			{
				// Only add to the focused stack if it matches the item being added
				if (FocusedEntry->ItemDefinition == Def)
				{
					const int32 Before = Remaining;
					Remaining = TryAddToEntry(*FocusedEntry, Remaining, MaxStackSize);
					bChanged |= (Remaining != Before);
				}
			}
		}

		// 2) Fill other existing stacks (skip focused one if already handled)
		TArray<FFortItemEntry*> Entries = FindItemEntries(Def);
		for (FFortItemEntry* Entry : Entries)
		{
			if (!Entry || Remaining <= 0)
				break;

			if (bHasFocusedGuid && Entry->ItemGuid == FocusedGuid)
				continue;

			const int32 Before = Remaining;
			Remaining = TryAddToEntry(*Entry, Remaining, MaxStackSize);
			bChanged |= (Remaining != Before);
		}

		// 3) Create new stacks while empty slots exist
		while (Remaining > 0 && !IsInventoryFull())
		{
			const int32 ToGive = UKismetMathLibrary::Min(Remaining, MaxStackSize); // smart

			FFortItemEntry NewEntry = ItemEntry;
			NewEntry.SetCount(ToGive);

			if (!AddItem(NewEntry))
				break;

			Remaining -= ToGive;
			bChanged = true;
		}
	}
	else if (IsSecondaryItem(Def))
	{
		if (FFortItemEntry* Entry = FindItemEntry(Def))
		{
			const int32 Before = Remaining;
			Remaining = TryAddToEntry(*Entry, Remaining, MaxStackSize);
			bChanged |= (Remaining != Before);
		}
		else
		{
			const int32 ToGive = UKismetMathLibrary::Min(Remaining, MaxStackSize);

			FFortItemEntry NewEntry = ItemEntry;
			NewEntry.SetCount(ToGive);

			if (AddItem(NewEntry))
			{
				Remaining -= ToGive;
				bChanged = true;
			}
		}
	}
	else
	{
		Log("AFortInventory::GetOverflowFromAddingItem: ItemDefinition has an invalid QuickBar type!");
		return Remaining;
	}

	if (bChanged)
		Update();

	return Remaining;
}

bool AFortInventory::RemoveItem(FGuid Guid, int32 Count, bool bDeferUpdate)
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
		return bDeferUpdate ? true : Update(Entry);
	}

	if (PC->IsUsingOldQuickBars() && floor(Version::Fortnite_Version) < 3)
	{
		PC->QuickBars->EmptyQuickbarSlot(Guid);
	}

	RemoveEntryAndInstance(Guid);

	return bDeferUpdate ? true : Update();
}

bool AFortInventory::RemoveItem(UFortItemDefinition* Def, int32 Count, bool bDeferUpdate)
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

	return RemoveItem(ItemEntry->ItemGuid, Count, bDeferUpdate);
}

void AFortInventory::RemoveEntryAndInstance(FGuid Guid)
{
	for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemGuid == Guid)
		{
			Inventory.ReplicatedEntries.RemoveAt(i, FFortItemEntry::GetSize());
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

	if (Version::Fortnite_Version >= 1.9 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11)
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

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		return 0;
	}

	int32 InventoryUsed = 0;

	if (Version::Fortnite_Version >= 1.9 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11)
	{
		IFortInventoryOwnerInterface* InventoryOwner = (IFortInventoryOwnerInterface*)Owner->GetInterfaceAddress(IFortInventoryOwnerInterface::StaticClass());
		if (!InventoryOwner)
		{
			Log("AFortInventory::GetInventoryUsed: Owner does not implement IFortInventoryOwnerInterface!");
			return 0;
		}

		int32(*GetInventoryUsedInternal)(IFortInventoryOwnerInterface*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
		InventoryUsed = GetInventoryUsedInternal(InventoryOwner, InventoryType);
	}
	else {
		int32(*GetInventoryUsedInternal)(AActor*, uint8) = decltype(GetInventoryUsedInternal)(ImageBase + Finder::FindAFortInventory_GetInventoryUsed());
		InventoryUsed = GetInventoryUsedInternal(Owner, InventoryType);
	}

	// semi fallback just incase the above fails
	if (InventoryUsed <= 0) {
		InventoryUsed = 0;

		for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++) {
			FFortItemEntry& ItemEntry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
			if (ItemEntry.ItemDefinition && ItemEntry.ItemDefinition->GetQuickBarForItem() == EFortQuickBars::GetPrimary()) {
				InventoryUsed += 1;
			}
		}
	}

	return InventoryUsed;
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

FFortItemEntry* AFortInventory::SwapCurrentItem(const FFortItemEntry& NewItemEntry, bool bSpawnPickup, bool bExecuteItem)
{
	UWorld* World = UWorld::GetWorld();
	if (!World)
		return nullptr;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		return nullptr;
	}

	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry)
		return nullptr;

	const FFortItemEntry OldItemEntry = *CurrentItemEntry;
	const FGuid CurrentGuid = CurrentItemEntry->ItemGuid;
	const int32 CurrentCount = CurrentItemEntry->Count;
	const uint8 CurrentQuickBar = CurrentItemEntry->ItemDefinition->GetQuickBarForItem();
	const int32 CurrentSlot = PC->IsUsingOldQuickBars()
		? PC->QuickBars->FindQuickBarSlotForItem(CurrentQuickBar, CurrentGuid)
		: PC->ClientQuickBars->FindQuickBarSlotForItem(CurrentQuickBar, CurrentGuid);

	if (PC->IsUsingOldQuickBars())
	{
		PC->QuickBars->EmptyQuickbarSlot(CurrentGuid);
	}

	for (int32 i = 0; i < Inventory.ReplicatedEntries.Num(); i++)
	{
		auto& Entry = Inventory.ReplicatedEntries.GetWithSize(i, FFortItemEntry::GetSize());
		if (Entry.ItemGuid == CurrentGuid)
		{
			Inventory.ReplicatedEntries.RemoveAt(i, FFortItemEntry::GetSize());
			break;
		}
	}

	for (int32 i = 0; i < Inventory.ItemInstances.Num(); i++)
	{
		UFortWorldItem* Item = Inventory.ItemInstances[i];
		if (Item && Item->ItemEntry.ItemGuid == CurrentGuid)
		{
			Inventory.ItemInstances.RemoveAt(i);
			break;
		}
	}

	FFortItemEntry* AddedEntry = AddItem(NewItemEntry);
	if (!AddedEntry)
	{
		AddItem(OldItemEntry);
		return nullptr;
	}

	if (bExecuteItem) {
		PC->ServerExecuteInventoryItem(PC, AddedEntry->ItemGuid);
		if (PC->IsUsingOldQuickBars())
		{
			PC->QuickBars->EquipItem(AddedEntry->ItemGuid);
		}

		PC->ClientExecuteInventoryItem(AddedEntry->ItemGuid, 0.f, true, true);
	}

	if (bSpawnPickup)
	{
		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			OldItemEntry.ItemDefinition,
			OldItemEntry.Count,
			PC->Pawn->K2_GetActorLocation(),
			PC->GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetTossedByPlayer(),
			PC,
			false
		);

		Pickup->PrimaryPickupItemEntry.LoadedAmmo = OldItemEntry.LoadedAmmo;
		Pickup->PrimaryPickupItemEntry.Durability = OldItemEntry.Durability;
		Pickup->PrimaryPickupItemEntry.bIsDirty = true;

		Pickup->PrimaryPickupItemEntry.ReplicationKey++;
		Pickup->OnRep_PrimaryPickupItemEntry();
	}

	return Update(AddedEntry) ? AddedEntry : nullptr;
}

bool AFortInventory::AddItemAndHandleOverflow(const FFortItemEntry& ItemEntry, bool bAllowSwap, bool bSpawnOverflowPickup)
{
	UWorld* World = UWorld::GetWorld();
	if (!World)
		return false;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		return false;
	}

	int32 Overflow = GetOverflowFromAddingItem(ItemEntry);
	if (Overflow <= 0)
		return true;

	FFortItemEntry OverflowEntry = ItemEntry;
	OverflowEntry.SetCount(Overflow);

	if (OverflowEntry.Count == ItemEntry.Count) {
		if (bAllowSwap && CanSwapForItem(OverflowEntry.ItemDefinition))
		{
			FFortItemEntry* AddedEntry = SwapCurrentItem(OverflowEntry, bSpawnOverflowPickup);
			if (AddedEntry)
			{
				//Log("AFortInventory::AddItemAndHandleOverflow: Swapped current item for new item: " + AddedEntry->ItemDefinition->GetName().ToString());
				
				return true;
			}
		}
	}

	if (bSpawnOverflowPickup)
	{
		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			OverflowEntry.ItemDefinition,
			OverflowEntry.Count,
			PC->Pawn->K2_GetActorLocation(),
			PC->GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetTossedByPlayer(),
			PC,
			false
		);

		Pickup->PrimaryPickupItemEntry.LoadedAmmo = OverflowEntry.LoadedAmmo;
		Pickup->PrimaryPickupItemEntry.Durability = OverflowEntry.Durability;
		Pickup->PrimaryPickupItemEntry.bIsDirty = true;

		Pickup->PrimaryPickupItemEntry.ReplicationKey++;
		Pickup->OnRep_PrimaryPickupItemEntry();

		return true;
	}

	return false;
}

bool AFortInventory::AddItemAndHandleOverflow(UFortItemDefinition* Def, int32 Count, bool bAllowSwap, bool bSpawnOverflowPickup)
{
	if (!CanAddItem(Def, Count))
		return false;

	FFortItemEntry ItemEntry = *FFortItemEntry::Allocate();
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

bool AFortInventory::DropAllItems(bool bSpawnPickups, bool bUseTossDirection)
{
	UWorld* World = UWorld::GetWorld();
	if (!World)
		return false;

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

		EntriesToRemove.Add(Entry, FFortItemEntry::GetSize());
	}

	for (int32 i = 0; i < EntriesToRemove.Num(); i++)
	{
		auto& Entry = EntriesToRemove.GetWithSize(i, FFortItemEntry::GetSize());
		if (bSpawnPickups)
		{
			AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
				World,
				Entry.ItemDefinition,
				Entry.Count,
				PC->Pawn->K2_GetActorLocation(),
				bUseTossDirection ? PC->GetDropFinalLocation() : *FVector::Allocate(),
				-1,
				true,
				true,
				true,
				-1,
				EFortPickupSourceTypeFlag::GetPlayer(),
				EFortPickupSpawnSource::GetUnset(),
				PC,
				false
			);

			Pickup->PrimaryPickupItemEntry.LoadedAmmo = Entry.LoadedAmmo;
			Pickup->PrimaryPickupItemEntry.Durability = Entry.Durability;
			Pickup->PrimaryPickupItemEntry.bIsDirty = true;

			Pickup->PrimaryPickupItemEntry.ReplicationKey++;
			Pickup->OnRep_PrimaryPickupItemEntry();
		}
		RemoveItem(Entry.ItemGuid, Entry.Count);
	}

	return true;
}

bool AFortInventory::CanAddItemWithStacking(UFortItemDefinition* Def, int32 Count) {
	if (!Def)
	{
		Log("AFortInventory::CanAddItem: ItemDefinition is null!");
		return false;
	}

	if (!IsInventoryFull()) {
		return true;
	}

	TArray<FFortItemEntry*> ExistingEntries = FindItemEntries(Def);
	if (ExistingEntries.Num() == 0)
		return true;

	for (FFortItemEntry* Entry : ExistingEntries)
	{
		if (!Entry)
			continue;
		if (Entry->Count < Def->GetMaxStackSize() && (Count == -1 || Count <= Def->GetMaxStackSize() - Entry->Count))
			return true;
	}

	return false;
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

	const int32 Current = UKismetMathLibrary::Max(0, ItemEntry.Count);
	const int32 Space = MaxStackSize - Current;
	if (Space <= 0)
		return Count;

	const int32 ToAdd = UKismetMathLibrary::Min(Count, Space);
	ItemEntry.SetCount(Current + ToAdd);
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
		PC->GetDropFinalLocation(),
		-1,
		true,
		true,
		true,
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

	return true;
}