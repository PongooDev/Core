#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EWorldItemDropBehavior {
public:
	DefineUnrealEnum(EWorldItemDropBehavior);

	DefineEnumProperty(DropAsPickup);
	DefineEnumProperty(DestroyOnDrop);
	DefineEnumProperty(DropAsPickupDestroyOnEmpty);
};

class UFortWorldItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortWorldItemDefinition);

	DefineUProperty(EWorldItemDropBehavior, DropBehavior);
	DefineBitfieldUProperty(bShouldShowItemToast);
	DefineBitfieldUProperty(bCanBeDropped);
	DefineBitfieldUProperty(bPersistInInventoryWhenFinalStackEmpty);
	DefineBitfieldUProperty(bAllowedToBeLockedInInventory);
	DefineBitfieldUProperty(bForceStayInOverflow);
	DefineBitfieldUProperty(bDropOnDeath);
	DefineBitfieldUProperty(bDropOnLogout);
	DefineBitfieldUProperty(bSupportsQuickbarFocus);
	DefineBitfieldUProperty(bSupportsQuickbarFocusForGamepadOnly);
	DefineBitfieldUProperty(bShouldActivateWhenFocused);
	DefineBitfieldUProperty(bForceFocusWhenAdded);
	DefineBitfieldUProperty(bShowDirectionalArrowWhenFarOff);
	DefineBitfieldUProperty(bItemCanBeStolen);
	DefineBitfieldUProperty(bCanBeDepositedInStorageVault);
	DefineBitfieldUProperty(bItemHasDurability);
	DefineBitfieldUProperty(bOverridePickupMeshTransform);
	DefineUProperty(bool, bCanAutoEquipByClass);
	DefineUProperty(int32, PreferredQuickbarSlot);
public:
	bool ServerExecute(UFortItem* Item, AFortPlayerController* Instigator);

	UFortWorldItemDefinition* GetAmmoWorldItemDefinition_BP() const;
};
