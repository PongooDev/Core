#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_FallDamageMultiplier : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_FallDamageMultiplier);

	DefineUProperty(float, FallDamageMultiplier);
};
