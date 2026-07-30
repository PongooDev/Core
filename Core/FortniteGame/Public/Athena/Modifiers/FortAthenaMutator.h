#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/FortGameplayMutator.h"

class AFortGameModeAthena;
class AFortGameStateAthena;

class AFortAthenaMutator : public AFortGameplayMutator {
public:
	DefineUnrealClass(AFortAthenaMutator);

	DefineUProperty(AFortGameModeAthena*, CachedGameMode);
	DefineUProperty(AFortGameStateAthena*, CachedGameState);
};
