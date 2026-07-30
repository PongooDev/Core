#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_GameDifficulty : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_GameDifficulty);

	DefineUProperty(FScalableFloat, GameDifficultyOverride);
};
