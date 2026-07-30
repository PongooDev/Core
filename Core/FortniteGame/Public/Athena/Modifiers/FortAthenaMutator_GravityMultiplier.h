#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_GravityMultiplier : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GravityMultiplier);

	DefineUProperty(int32, GravityOverride);
	DefineUProperty(TArray<FGravityMovementData>, CustomGravityData);
};
