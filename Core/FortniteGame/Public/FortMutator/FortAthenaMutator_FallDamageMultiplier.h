#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_FallDamageMultiplier : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_FallDamageMultiplier);

	DefineUProperty(float, FallDamageMultiplier);
};
