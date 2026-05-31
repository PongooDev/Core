#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

#include "FortniteGame/Public/FortItem/FortItem.h"
#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/FortItem/FortItemList.h"

class UFortItemDefinition;
struct FFortItemEntry;
class AFortPickup;
class AFortPlayerController;
class IFortInventoryOwnerInterface;
class UFortWorldItemDefinition;
class AFortQuickBars;

class AFortInventory : public AActor
{
public:
	DefineUnrealClass(AFortInventory);

	DefineUProperty(FFortItemList, Inventory);

	DefineUProperty(bool, bRequiresLocalUpdate);
	DefineUProperty(bool, bRequiresSaving);
	DefineUProperty(uint8, InventoryType);

public:
	// =================================================================
	// Inventory Lifecycle
	// =================================================================

	void HandleInventoryLocalUpdate();
	bool Update(FFortItemEntry* ItemEntry = nullptr);
	void InitializeExistingItem(UFortWorldItem* ExistingItem);

	// =================================================================
	// Inventory Lookup
	// =================================================================

	FFortItemEntry* FindItemEntry(FGuid Guid);
	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);
	FFortItemEntry* FindItemEntry(int32 ItemType);
	FFortItemEntry* FindItemEntry(UClass* ItemDefinitionClass);

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	TArray<FFortItemEntry*> FindItemEntries(UFortItemDefinition* ItemDefinition);
	TArray<UFortWorldItem*> FindItemInstances(UFortItemDefinition* ItemDefinition);

	FFortItemEntry* GetCurrentItemEntry();
	AFortPlayerController* GetOwnerPlayerController() const;

	// =================================================================
	// Inventory Mutation
	// =================================================================

	FFortItemEntry* AddItem(UFortWorldItem* Item);
	FFortItemEntry* AddItem(UFortItemDefinition* Def, int32 Count = 1, int32 Level = 0);
	FFortItemEntry* AddItem(const FFortItemEntry& ItemEntry);

	int32 GetOverflowFromAddingItem(const FFortItemEntry& ItemEntry);

	bool RemoveItem(FGuid Guid, int32 Count = INT_MAX);
	bool RemoveItem(UFortItemDefinition* Def, int32 Count = INT_MAX);

	void RemoveEntryAndInstance(FGuid Guid);

	// =================================================================
	// Inventory State
	// =================================================================

	int32 GetInventoryCapacity();
	int32 GetInventoryUsed();
	bool IsInventoryFull();

	// =================================================================
	// High-Level Item Handling
	// =================================================================

	bool CanSwapForItem(UFortItemDefinition* Def);

	FFortItemEntry* SwapCurrentItem(
		const FFortItemEntry& NewItemEntry,
		bool bSpawnPickup = true
	);

	bool AddItemAndHandleOverflow(
		const FFortItemEntry& ItemEntry,
		bool bAllowSwap = true,
		bool bSpawnOverflowPickup = true
	);
	bool AddItemAndHandleOverflow(
		UFortItemDefinition* Def,
		int32 Count,
		bool bAllowSwap = true,
		bool bSpawnOverflowPickup = true
	);

	bool IsCurrentItem(FGuid& ItemGuid);

	void EquipHarvestingTool();

	bool DropAllItems(bool bSpawnPickups = true);

	// =================================================================
	// Internal Helpers
	// =================================================================

	bool CanAddItem(UFortItemDefinition* Def, int32 Count = 1) const;
	bool CanAddItem(const FFortItemEntry& ItemEntry) const;
	bool CanRemoveItem(FGuid Guid, int32 Count) const;

	bool IsPrimaryItem(UFortItemDefinition* Def) const;
	bool IsSecondaryItem(UFortItemDefinition* Def) const;

	int32 TryAddToEntry(FFortItemEntry& ItemEntry, int32 Count = 1, int32 MaxStackSize = 1);
	int32 TryCreateStack(UFortItemDefinition* Def, int32 Count = 1, int32 MaxStackSize = 1);

	AFortPickup* SpawnPickupFromDefinition(UFortItemDefinition* Def, int32 Count = 1);
	AFortPickup* SpawnPickupFromEntry(const FFortItemEntry& ItemEntry);

	bool SetStateValues(FFortItemEntry* ItemEntry);
	bool SetNewItemCountStateValue(FFortItemEntry* ItemEntry, int32 NewCount);
};