#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_StartingInventory : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_StartingInventory);

	DefineUProperty(TArray<FItemAndCount>, StartingInventory);
};
