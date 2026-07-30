#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

class UFortWorldItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortWorldItemDefinition);

	DefineUProperty(EWorldItemDropBehavior, DropBehavior);
	DefineBitfieldUProperty(bShouldShowItemToast);
	DefineBitfieldUProperty(bCanBeDropped);
	DefineBitfieldUProperty(bPersistInInventoryWhenFinalStackEmpty);
	DefineBitfieldUProperty(bAllowedToBeLockedInInventory);
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
	DefineUProperty(int32, PreferredQuickbarSlot);
public:
	bool ServerExecute(UFortItem* Item, AFortPlayerController* Instigator);

	UFortWorldItemDefinition* GetAmmoWorldItemDefinition_BP() const;
};
