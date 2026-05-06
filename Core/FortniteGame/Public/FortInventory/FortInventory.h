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

class AFortInventory : public AActor {
public:
	DefineUnrealClass(AFortInventory);

	DefineUProperty(FFortItemList, Inventory);

	DefineUProperty(bool, bRequiresLocalUpdate);
	DefineUProperty(bool, bRequiresSaving);
	DefineUProperty(uint8, InventoryType);
public:
	void HandleInventoryLocalUpdate();

	FFortItemEntry* FindItemEntry(FGuid Guid);
	FFortItemEntry* FindItemEntry(UFortItemDefinition* ItemDefinition);

	UFortWorldItem* FindItemInstance(FGuid Guid);
	UFortWorldItem* FindItemInstance(UFortItemDefinition* ItemDefinition);

	UFortWorldItem* AddItem(UFortWorldItem* Item);
	UFortWorldItem* AddItem(UFortItemDefinition* Def, int32 Count = 1, int32 Level = 0);
	UFortWorldItem* AddItem(FFortItemEntry& ItemEntry);

	int32 GetOverflowFromAddingItem(UFortItemDefinition* Def, int32 Count = 1);

	bool Update(FFortItemEntry* ItemEntry = nullptr);

	void InitializeExistingItem(UFortWorldItem* ExistingItem);

	bool RemoveItem(FGuid Guid, int32 Count = INT_MAX);
	bool RemoveItem(UFortItemDefinition* Def, int32 Count = INT_MAX);

	int32 GetInventoryCapacity();

	int32 GetInventoryUsed();

	bool IsInventoryFull();
};