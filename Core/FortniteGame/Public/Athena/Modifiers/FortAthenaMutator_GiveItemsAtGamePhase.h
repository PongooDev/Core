#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_GiveItemsAtGamePhase : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GiveItemsAtGamePhase);

	DefineUProperty(EAthenaGamePhase, PhaseToGiveItems);
	DefineUProperty(TArray<FItemsToGiveAtPhase>, ItemsToGive);
};
