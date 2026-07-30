#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_SkyCap : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SkyCap);

	DefineUProperty(TSubclassOf<AAthenaSkyCap>, SkyCapClass);
	DefineUProperty(AAthenaSkyCap*, SkyCap);
	DefineUProperty(FScalableFloat, SpawnHeight);
	DefineUProperty(FScalableFloat, InitialDelay);
	DefineUProperty(TArray<FSkyCapPositionData>, PositionData);
};
