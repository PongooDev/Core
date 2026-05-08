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

void AFortInventory::HandleInventoryLocalUpdate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleInventoryLocalUpdate");

	ProcessEvent(Func, nullptr);
}

bool AFortInventory::Update(FFortItemEntry* ItemEntry)
{
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

FFortItemEntry* AFortInventory::GetCurrentItemEntry() {
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;

	AFortPawn* Pawn = PC->Pawn->Cast<AFortPawn>();
	if (!Pawn) {
		Log("AFortInventory::GetCurrentItemEntry: Pawn is null!");
		return nullptr;
	}

	AFortWeapon* CurrentWeapon = Pawn->CurrentWeapon;
	if (!CurrentWeapon) {
		Log("AFortInventory::GetCurrentItemEntry: CurrentWeapon is null!");
		return nullptr;
	}

	return FindItemEntry(CurrentWeapon->ItemEntryGuid);
}

AFortPlayerController* AFortInventory::GetOwnerPlayerController() const
{
	return Owner ? Owner->Cast<AFortPlayerController>() : nullptr;
}

UFortWorldItem* AFortInventory::AddItem(UFortWorldItem* Item)
{
	if (!Item)
		return nullptr;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return nullptr;
	
	Item->SetOwningControllerForTemporaryItem(PC);

	InitializeExistingItem(Item);

	if (Version::Fortnite_Version <= 1.8) {
		PC->QuickBars->AddItemToQuickBar(Item->ItemEntry.ItemGuid);
	}

	return Item;
}

UFortWorldItem* AFortInventory::AddItem(UFortItemDefinition* Def, int32 Count, int32 Level)
{
	if (!CanAddItem(Def, Count))
	{
		return nullptr;
	}

	UFortWorldItem* Item = (UFortWorldItem*)Def->CreateTemporaryItemInstanceBP(Count, Level);
	if (!Item)
	{
		Log("AFortInventory::AddItem: Failed to create temp item for " + Def->GetName().ToString());
		return nullptr;
	}

	return AddItem(Item);
}

UFortWorldItem* AFortInventory::AddItem(const FFortItemEntry& ItemEntry)
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

	UFortWorldItem* Item = AddItem(Def, ItemEntry.Count, ItemEntry.Level);
	if (!Item)
		return nullptr;

	if (FFortItemEntry* RepEntry = FindItemEntry(Item->ItemEntry.ItemGuid))
	{
		RepEntry->AlterationDefinitions = ItemEntry.AlterationDefinitions;
		RepEntry->AlterationInstances = ItemEntry.AlterationInstances;
		RepEntry->bUpdateStatsOnCollection = ItemEntry.bUpdateStatsOnCollection;
		RepEntry->ControlOverride = ItemEntry.ControlOverride;
		RepEntry->Durability = ItemEntry.Durability;
		RepEntry->GiftingInfo = ItemEntry.GiftingInfo;
		RepEntry->ItemSource = ItemEntry.ItemSource;
		RepEntry->LoadedAmmo = ItemEntry.LoadedAmmo;
		RepEntry->StateValues = ItemEntry.StateValues;

		Update(RepEntry);
	}
	else
	{
		Log("AFortInventory::AddItem: Could not find RepEntry for GUID " + Item->ItemEntry.ItemGuid.FormatGuid());
	}

	return Item;
}

int32 AFortInventory::GetOverflowFromAddingItem(UFortItemDefinition* Def, int32 Count) {
	if (!CanAddItem(Def, Count))
		return 0;

	const int32 MaxStackSize = Def->GetMaxStackSize();
	if (MaxStackSize <= 0)
		return Count;

	if (IsPrimaryItem(Def)) {
		if (!Def->IsStackable())
		{
			while (Count > 0 && !IsInventoryFull())
			{
				AddItem(Def, 1);
				--Count;
			}
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
			Count = TryCreateStack(Def, Count, MaxStackSize);
		}
	}
	else if (IsSecondaryItem(Def)) {
		FFortItemEntry* Entry = FindItemEntry(Def);
		if (Entry)
		{
			Count = TryAddToEntry(*Entry, Count, MaxStackSize);
		}
		else {
			Count = TryCreateStack(Def, Count, MaxStackSize);
		}
	}
	else {
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

	if (Version::Fortnite_Version <= 1.8) {
		PC->QuickBars->EmptyQuickbarSlot(Guid);
	}
	RemoveEntryAndInstance(Guid);

	return Update();
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

bool AFortInventory::CanSwapForItem(UFortItemDefinition* Def) {
	if (!IsPrimaryItem(Def))
		return false;

	if (!IsInventoryFull())
		return false;

	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry) {
		Log("AFortInventory::ShouldSwapCurrentItem: CurrentItemEntry is null!");
		return false;
	}

	UFortItemDefinition* CurrentItemDef = CurrentItemEntry->ItemDefinition;
	if (!CurrentItemDef)
	{
		Log("AFortInventory::ShouldSwapCurrentItem: CurrentItemDef is null!");
		return false;
	}

	const int32 CurrentCount = CurrentItemEntry->Count;

	if (!IsPrimaryItem(CurrentItemDef))
		return false;

	if (CurrentItemDef == Def && CurrentItemDef->IsStackable())
	{
		return CurrentCount >= CurrentItemDef->GetMaxStackSize();
	}

	return true;
}

bool AFortInventory::SwapCurrentItem(UFortItemDefinition* NewItemDef, int32 NewCount, bool bSpawnPickup) {
	if (!CanAddItem(NewItemDef, NewCount)) {
		Log("AFortInventory::SwapCurrentItem: Cannot add new item " + NewItemDef->GetName().ToString() + " with count " + std::to_string(NewCount));
		return false;
	}

	if (!CanSwapForItem(NewItemDef)) {
		Log("AFortInventory::SwapCurrentItem: Cannot swap for item " + NewItemDef->GetName().ToString());
		return false;
	}

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC) {
		Log("AFortInventory::SwapCurrentItem: PlayerController is null!");
		return false;
	}

	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry) {
		Log("AFortInventory::SwapCurrentItem: CurrentItemEntry is null!");
		return false;
	}

	UFortItemDefinition* CurrentItemDef = CurrentItemEntry->ItemDefinition;
	if (!CurrentItemDef) {
		Log("AFortInventory::SwapCurrentItem: CurrentItemDef is null!");
		return false;
	}

	const FGuid CurrentGuid = CurrentItemEntry->ItemGuid;
	const int32 CurrentCount = CurrentItemEntry->Count;

	if (!RemoveItem(CurrentGuid, CurrentCount)) {
		Log("AFortInventory::SwapCurrentItem: Failed to remove current item from inventory!");
		return false;
	}

	if (bSpawnPickup)
	{
		SpawnPickupFromDefinition(CurrentItemDef, CurrentCount);
	}

	UFortWorldItem* NewItem = AddItem(NewItemDef, NewCount);
	if (!NewItem) {
		Log("AFortInventory::SwapCurrentItem: Failed to add new item to inventory!");
		return false;
	}

	return true;
}

bool AFortInventory::AddItemAndHandleOverflow(UFortItemDefinition* Def, int32 Count, bool bAllowSwap, bool bSpawnOverflowPickup)
{
	if (!CanAddItem(Def, Count))
		return false;

	int32 Overflow = GetOverflowFromAddingItem(Def, Count);
	if (Overflow <= 0)
		return true;

	if (bAllowSwap && CanSwapForItem(Def))
	{
		if (SwapCurrentItem(Def, Count, bSpawnOverflowPickup))
		{
			return true;
		}
	}

	if (bSpawnOverflowPickup)
	{
		SpawnPickupFromDefinition(Def, Overflow);
		return true;
	}

	return false;
}

bool AFortInventory::IsCurrentItem(FGuid& ItemGuid) {
	FFortItemEntry* CurrentItemEntry = GetCurrentItemEntry();
	if (!CurrentItemEntry)
		return false;

	return CurrentItemEntry->ItemGuid == ItemGuid;
}

void AFortInventory::EquipHarvestingTool() {
	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return;

	FFortItemEntry* PickaxeEntry = FindItemEntry(EFortItemType::GetWeaponHarvest());
	if (!PickaxeEntry) {
		Log("AFortInventory::EquipPickaxe: Pickaxe not found in inventory!");
		return;
	}

	PC->ServerExecuteInventoryItem(PC, PickaxeEntry->ItemGuid);
	if (Version::Fortnite_Version <= 1.8) {
		PC->QuickBars->EquipHarvestingTool();
	}
}

bool AFortInventory::CanAddItem(UFortItemDefinition* Def, int32 Count) const
{
	return Owner && Def && Count > 0;
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
	ItemEntry.Count += ToAdd;
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

bool AFortInventory::SpawnPickupFromDefinition(UFortItemDefinition* Def, int32 Count)
{
	if (!CanAddItem(Def, Count))
		return false;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return false;

	AFortPlayerController* PC = GetOwnerPlayerController();
	if (!PC)
		return false;

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
		EFortPickupSourceTypeFlag::Player,
		EFortPickupSpawnSource::Unset,
		PC,
		false
	) != nullptr;
}

bool AFortInventory::SpawnPickupFromEntry(const FFortItemEntry& ItemEntry)
{
	return SpawnPickupFromDefinition(ItemEntry.ItemDefinition, ItemEntry.Count);
}