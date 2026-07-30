#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_InventoryOverride : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_InventoryOverride);

	DefineUProperty(EAthenaLootDropOverride, DropAllItemsOverride);
	DefineUProperty(EAthenaLootDropOverride, TrapDropOverride);
	DefineUProperty(EAthenaLootDropOverride, WeaponDropOverride);
	DefineUProperty(EAthenaLootDropOverride, MaterialDropOverride);
	DefineUProperty(EAthenaLootDropOverride, GadgetDropOverride);
	DefineUProperty(EAthenaLootDropOverride, ConsumableDropOverride);
	DefineUProperty(EAthenaLootDropOverride, AmmoDropOverride);
	DefineUProperty(EAthenaInventorySpawnOverride, InventoryUpdateOverride);
	DefineUProperty(TArray<FItemLoadoutContainer>, InventoryLoadouts);
	DefineUProperty(int32, InventoryLoadoutIndex);
	DefineUProperty(TArray<FItemLoadoutTeamMap>, TeamLoadouts);
};
