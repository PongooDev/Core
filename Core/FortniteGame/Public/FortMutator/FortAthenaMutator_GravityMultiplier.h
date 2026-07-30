#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_GravityMultiplier : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GravityMultiplier);

	DefineUProperty(int32, GravityOverride);
	DefineUProperty(TArray<FGravityMovementData>, CustomGravityData);
};
