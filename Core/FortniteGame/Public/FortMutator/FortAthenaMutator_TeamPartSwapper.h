#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_TeamPartSwapper : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TeamPartSwapper);

	DefineUProperty(TArray<uint8>, TeamNums);
	DefineUProperty(TArray<FPartSwapData>, SwapData);
};
