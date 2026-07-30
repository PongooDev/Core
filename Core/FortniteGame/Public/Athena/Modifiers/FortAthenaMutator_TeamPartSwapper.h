#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_TeamPartSwapper : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TeamPartSwapper);

	DefineUProperty(TArray<uint8>, TeamNums);
	DefineUProperty(TArray<FPartSwapData>, SwapData);
};
