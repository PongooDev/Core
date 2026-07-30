#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_EnvironmentalDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_EnvironmentalDamage);

	DefineUProperty(FGameplayTagQuery, DamageTagQuery);
	DefineUProperty(float, DamageMultiplier);
	DefineUProperty(bool, bExcludeActorsAddedToVolume);
	DefineUProperty(float, WorldActivationTime);
};
