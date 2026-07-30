#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_WarmupTime : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_WarmupTime);

	DefineUProperty(float, WarmupTime);
};
