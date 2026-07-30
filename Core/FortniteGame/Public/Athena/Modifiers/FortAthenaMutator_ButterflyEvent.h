#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_ButterflyEvent : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_ButterflyEvent);

	DefineUProperty(float, PawnGatherFrequency);
	DefineUProperty(FScalableFloat, PawnSpawnLocationX);
	DefineUProperty(FScalableFloat, PawnSpawnLocationY);
	DefineUProperty(FScalableFloat, PawnSpawnMinDistance);
	DefineUProperty(FScalableFloat, PawnSpawnMaxDistance);
};
