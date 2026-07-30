#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_StartingInventory : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_StartingInventory);

	DefineUProperty(TArray<FItemAndCount>, StartingInventory);
};
