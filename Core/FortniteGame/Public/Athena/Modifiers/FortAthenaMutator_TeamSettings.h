#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

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
