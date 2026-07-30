#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Score : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Score);

	DefineUProperty(int32, NumCoinWaves);
	DefineUProperty(bool, bSupportsRespawnConfig);
	DefineUProperty(bool, bRespawnsAllowed);
	DefineUProperty(FScalableFloat, StopRespawnPhase);
	DefineUProperty(FText, GameMsgText_Intro);
	DefineUProperty(FText, GameMsgText_FirstCoinsSpawned);
	DefineUProperty(FText, GameMsgText_CoinsSpawned);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawnsWarning);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawns);
};
