#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_TimeStep : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TimeStep);

	DefineUProperty(float, TimeStep);
};
