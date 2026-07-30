#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_TODOverride : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TODOverride);

	DefineUProperty(EAthenaTimeOfDayOverride, TimeOfDayOverride);
	DefineUProperty(float, TimeOverride);
	DefineUProperty(float, SpeedOverride);
	DefineUProperty(TArray<FTimeOfDayPhase>, TimeOfDayPhases);
	DefineUProperty(TArray<FTimeOfDaySpeed>, TimeOfDaySpeeds);
};
