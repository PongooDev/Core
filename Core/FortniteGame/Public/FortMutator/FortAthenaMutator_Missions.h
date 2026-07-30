#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Missions : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Missions);

	DefineUProperty(TArray<UFortMissionInfo*>, Missions);
};
