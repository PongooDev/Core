#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_PlayerDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PlayerDamage);

	DefineUProperty(FGameplayTagQuery, DamageTagQuery);
	DefineUProperty(float, DamageMultiplier);
};
