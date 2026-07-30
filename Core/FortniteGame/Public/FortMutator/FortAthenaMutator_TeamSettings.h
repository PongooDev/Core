#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

// The comma would split DefineUProperty into three macro arguments.
using FTeamSettingsDataMap = TMap<uint8, FTeamSettingsData>;

class AFortAthenaMutator_TeamSettings : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TeamSettings);

	DefineUProperty(TSubclassOf<UFortTeamSettingsComponent>, TeamSettingsClass);
	DefineUProperty(TArray<FGravityMovementData>, GravityData);
	DefineUProperty(FTeamSettingsDataMap, TeamSettingsDataMap);
	DefineUProperty(UFortTeamSettingsComponent*, DefaultTeamSettings);
	DefineUProperty(UFortTeamSettingsComponent*, CurrentTeamSettings);
};
