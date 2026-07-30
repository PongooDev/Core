#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_VehicleTrickScore : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_VehicleTrickScore);

	DefineUProperty(float, ScoreMultiplier);
};
