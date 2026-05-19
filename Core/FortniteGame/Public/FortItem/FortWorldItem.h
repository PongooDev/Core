#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

#include "FortItemEntry.h"
#include "FortItem.h"
#include "FortniteGame/Public/FortEnums.h"

class AFortPlayerController;
class UFortItemDefinition;
class UFortAccountItemDefinition;
class AFortInventory;

class UFortWorldItem : public UFortItem {
public:
	DefineUnrealClass(UFortWorldItem);

	DefineUProperty(FFortItemEntry, ItemEntry);
	DefineUProperty(AFortInventory*, OwnerInventory);
public:
	uint8 GetQuickBarForItem();

	void SetOwningInventory(AFortInventory* NewOwnerInventory);

	static bool SetLoadedAmmo(UFortWorldItem* This, int32 InCount);

	static bool SetPhantomReserveAmmo(UFortWorldItem* This, int32 InCount);

	static bool SetInInventoryOverflow(UFortWorldItem* This, bool bInInventoryOverflow);

	static bool SetDurability(UFortWorldItem* This, float InDurability);

	static void Hook() {
		HookEveryVTableIdx(
			UFortWorldItem::StaticClass(),
			Finder::FindUFortWorldItem_SetLoadedAmmoVFT(),
			SetLoadedAmmo
		);

		if (Finder::FindUFortWorldItem_SetPhantomReserveAmmoVFT()) {
			HookEveryVTableIdx(
				UFortWorldItem::StaticClass(),
				Finder::FindUFortWorldItem_SetPhantomReserveAmmoVFT(),
				SetPhantomReserveAmmo
			);
		}

		HookEveryVTableIdx(
			UFortWorldItem::StaticClass(),
			Finder::FindUFortWorldItem_SetInInventoryOverflowVFT(),
			SetInInventoryOverflow
		);

		HookEveryVTableIdx(
			UFortWorldItem::StaticClass(),
			Finder::FindUFortWorldItem_SetDurabilityVFT(),
			SetDurability
		);

		Log("UFortWorldItem Hooked!");
	}
};