#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_GameDifficulty : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GameDifficulty);

	DefineUProperty(FScalableFloat, GameDifficultyOverride);
};
