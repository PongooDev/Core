#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Missions : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Missions);

	DefineUProperty(TArray<UFortMissionInfo*>, Missions);
};
