#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_RespawnLocation : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_RespawnLocation);

	DefineUProperty(bool, bRespawnInAir);
	DefineUProperty(FGameplayTagQuery, LocationQuery);
	DefineUProperty(bool, bRespawnAtDeath);
	DefineUProperty(float, RespawnHeight);
};
