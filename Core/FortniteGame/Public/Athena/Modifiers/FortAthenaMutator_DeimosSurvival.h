#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_DeimosSurvival : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_DeimosSurvival);

	DefineUProperty(TSoftObjectPtr<UEnvQuery>, AdjustSafezoneCentersQuery);
	DefineUProperty(TArray<FFortAthenaMutator_SurvivalObjectiveData>, ObjectivesData);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
	DefineUProperty(FAthenaGameMessageData, GameMsg_CompleteObjective);
	DefineUProperty(FAthenaGameMessageData, GameMsg_DownTime);
};
