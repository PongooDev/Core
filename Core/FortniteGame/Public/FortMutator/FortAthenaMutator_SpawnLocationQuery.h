#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_SpawnLocationQuery : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SpawnLocationQuery);

	DefineUProperty(FGameplayTagQuery, LocationQuery);
};
