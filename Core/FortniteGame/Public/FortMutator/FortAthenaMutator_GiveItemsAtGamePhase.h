#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_GiveItemsAtGamePhase : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GiveItemsAtGamePhase);

	DefineUProperty(EAthenaGamePhase, PhaseToGiveItems);
	DefineUProperty(TArray<FItemsToGiveAtPhase>, ItemsToGive);
};
