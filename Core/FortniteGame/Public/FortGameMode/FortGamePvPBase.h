#pragma once
#include "pch.h"

#include "FortGameModeZone.h"

class AFortGamePvPBase : public AFortGameModeZone {
public:
	DefineUnrealClass(AFortGamePvPBase);

	DefineUProperty(uint8, NumTeams);
};