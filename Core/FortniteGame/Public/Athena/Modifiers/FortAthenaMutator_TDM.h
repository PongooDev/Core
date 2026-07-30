#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_TDM : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TDM);

	DefineUProperty(FScalableFloat, NumKillsForVictory);
	DefineUProperty(bool, bIsTwoTeamTDM);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
};
