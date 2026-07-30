#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_GiveItemsAtGamePhaseStep : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GiveItemsAtGamePhaseStep);

	DefineUProperty(EAthenaGamePhaseStep, PhaseToGiveItems);
	DefineUProperty(TArray<FItemsToGive>, ItemsToGive);
};
