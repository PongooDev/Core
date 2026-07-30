#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_VehicleTrickScore : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_VehicleTrickScore);

	DefineUProperty(float, ScoreMultiplier);
};
