#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_PlayerFly : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PlayerFly);

	DefineUProperty(bool, bFlyEnabled);
};
