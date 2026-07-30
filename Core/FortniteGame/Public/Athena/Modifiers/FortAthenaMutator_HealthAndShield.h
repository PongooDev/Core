#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_HealthAndShield : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_HealthAndShield);

	DefineUProperty(EAthenaMutatorEvaluators, NumericalMutatorOverride);
	DefineUProperty(float, MaxHealth);
	DefineUProperty(float, StartingHealth);
	DefineUProperty(float, MaxShield);
	DefineUProperty(float, StartingShield);
};
