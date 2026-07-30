#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_InfiniteResources : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_InfiniteResources);

	DefineUProperty(bool, bInfiniteResources);
};
