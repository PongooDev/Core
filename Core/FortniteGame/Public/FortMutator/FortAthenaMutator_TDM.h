#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_TDM : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TDM);

	DefineUProperty(FScalableFloat, NumKillsForVictory);
	DefineUProperty(bool, bIsTwoTeamTDM);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
};
