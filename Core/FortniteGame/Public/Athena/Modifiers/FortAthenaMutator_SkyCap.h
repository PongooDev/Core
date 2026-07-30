#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_SkyCap : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SkyCap);

	DefineUProperty(TSubclassOf<AAthenaSkyCap>, SkyCapClass);
	DefineUProperty(AAthenaSkyCap*, SkyCap);
	DefineUProperty(FScalableFloat, SpawnHeight);
	DefineUProperty(FScalableFloat, InitialDelay);
	DefineUProperty(TArray<FSkyCapPositionData>, PositionData);
};
