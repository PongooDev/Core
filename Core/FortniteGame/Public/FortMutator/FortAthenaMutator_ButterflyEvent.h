#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_ButterflyEvent : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_ButterflyEvent);

	DefineUProperty(float, PawnGatherFrequency);
	DefineUProperty(FScalableFloat, PawnSpawnLocationX);
	DefineUProperty(FScalableFloat, PawnSpawnLocationY);
	DefineUProperty(FScalableFloat, PawnSpawnMinDistance);
	DefineUProperty(FScalableFloat, PawnSpawnMaxDistance);
};
