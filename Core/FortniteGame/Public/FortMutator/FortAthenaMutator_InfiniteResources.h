#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_InfiniteResources : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_InfiniteResources);

	DefineUProperty(bool, bInfiniteResources);
};
