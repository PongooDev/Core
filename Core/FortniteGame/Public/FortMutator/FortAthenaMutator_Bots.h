#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator_SpawningPolicySafezone.h"

class AFortAthenaMutator_Bots : public AFortAthenaMutator_SpawningPolicySafezone {
public:
	DefineUnrealClass(AFortAthenaMutator_Bots);

	DefineUProperty(bool, bBotHostileToHumanPlayersOnly);
	DefineUProperty(TSubclassOf<UFortServerBotManagerAthena>, ServerBotManagerClass);
};
