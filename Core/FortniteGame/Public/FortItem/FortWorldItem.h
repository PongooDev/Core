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

class UFortWorldItem : public UFortItem {
public:
	DefineUnrealClass(UFortWorldItem);

	DefineUProperty(FFortItemEntry, ItemEntry);
public:
	uint8 GetQuickBarForItem();

	void SetOwningInventory(AFortInventory* NewOwnerInventory);

	static bool SetLoadedAmmo(UFortWorldItem* This, int32 InCount);

	static void Hook() {
		HookEveryVTableIdx(
			UFortWorldItem::StaticClass(),
			Finder::FindUFortWorldItem_SetLoadedAmmoVFT(),
			SetLoadedAmmo
		);

		Log("UFortWorldItem Hooked!");
	}
};