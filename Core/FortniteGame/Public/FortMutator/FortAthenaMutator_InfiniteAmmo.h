#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_InfiniteAmmo : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_InfiniteAmmo);

	DefineUProperty(bool, bInfiniteAmmo);
};
