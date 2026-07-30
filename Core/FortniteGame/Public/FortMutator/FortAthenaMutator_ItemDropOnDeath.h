#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_ItemDropOnDeath : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_ItemDropOnDeath);

	DefineUProperty(ERespawnRequirements, RespawnRequirements);
	DefineUProperty(FScalableFloat, PercentAmmoToDrop);
	DefineUProperty(FScalableFloat, AmmoDespawnTime);
	DefineUProperty(FScalableFloat, AmmoStormDespawnTime);
	DefineUProperty(FScalableFloat, MaxAmmoTypeDrops);
	DefineUProperty(FScalableFloat, MaxResourceTypeDrops);
	DefineUProperty(TArray<FItemsToDropOnDeath>, ItemsToDrop);
};
