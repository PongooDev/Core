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

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	TArray<FFortItemEntry*> FindItemEntries(UFortItemDefinition* ItemDefinition);
	TArray<UFortWorldItem*> FindItemInstances(UFortItemDefinition* ItemDefinition);

	FFortItemEntry* GetCurrentItemEntry();
	AFortPlayerController* GetOwnerPlayerController() const;

	// =================================================================
	// Inventory Mutation
	// =================================================================

	UFortWorldItem* AddItem(UFortWorldItem* Item);
	UFortWorldItem* AddItem(UFortItemDefinition* Def, int32 Count = 1, int32 Level = 0);
	UFortWorldItem* AddItem(const FFortItemEntry& ItemEntry);

	int32 GetOverflowFromAddingItem(UFortItemDefinition* Def, int32 Count = 1);

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

	bool SwapCurrentItem(
		UFortItemDefinition* NewItemDef,
		int32 NewCount,
		bool bSpawnPickup = true
	);

	bool AddItemAndHandleOverflow(
		UFortItemDefinition* Def,
		int32 Count = 1,
		bool bAllowSwap = true,
		bool bSpawnOverflowPickup = true
	);

	bool IsCurrentItem(FGuid& ItemGuid);

	void EquipHarvestingTool();

	// =================================================================
	// Internal Helpers
	// =================================================================

	bool CanAddItem(UFortItemDefinition* Def, int32 Count = 1) const;
	bool CanRemoveItem(FGuid Guid, int32 Count) const;

	bool IsPrimaryItem(UFortItemDefinition* Def) const;
	bool IsSecondaryItem(UFortItemDefinition* Def) const;

	int32 TryAddToEntry(FFortItemEntry& ItemEntry, int32 Count = 1, int32 MaxStackSize = 1);
	int32 TryCreateStack(UFortItemDefinition* Def, int32 Count = 1, int32 MaxStackSize = 1);

	bool SpawnPickupFromDefinition(UFortItemDefinition* Def, int32 Count = 1);
	bool SpawnPickupFromEntry(const FFortItemEntry& ItemEntry);
};