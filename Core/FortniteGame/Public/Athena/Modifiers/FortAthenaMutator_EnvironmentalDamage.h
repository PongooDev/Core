#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_EnvironmentalDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_EnvironmentalDamage);

	DefineUProperty(FGameplayTagQuery, DamageTagQuery);
	DefineUProperty(float, DamageMultiplier);
	DefineUProperty(bool, bExcludeActorsAddedToVolume);
	DefineUProperty(float, WorldActivationTime);
};
