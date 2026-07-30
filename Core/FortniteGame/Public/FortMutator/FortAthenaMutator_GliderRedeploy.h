#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_GliderRedeploy : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GliderRedeploy);

	DefineUProperty(bool, bGliderRedeploy);
};
