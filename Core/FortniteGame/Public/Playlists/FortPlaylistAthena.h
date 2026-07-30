#pragma once
#include "pch.h"

#include "FortniteGame/Public/Playlists/FortPlaylist.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAthenaAISettings;

class EAthenaRespawnType {
public:
	DefineUnrealEnum(EAthenaRespawnType);

	DefineEnumProperty(None);
	DefineEnumProperty(InfiniteRespawn);
	DefineEnumProperty(InfiniteRespawnExceptStorm);
};

class EAthenaWinCondition {
public:
	DefineUnrealEnum(EAthenaWinCondition);

	DefineEnumProperty(LastManStanding);
	DefineEnumProperty(LastManStandingIncludingAllies);
	DefineEnumProperty(TimedTeamFinalFight);
	DefineEnumProperty(FirstToGoalScore);
	DefineEnumProperty(TimedLastMenStanding);
	DefineEnumProperty(MutatorControlled);
};

class UFortPlaylistAthena : public UFortPlaylist {
public:
	DefineUnrealClass(UFortPlaylistAthena);

	DefineUProperty(uint8, WinConditionType);
	DefineUProperty(uint8, SafeZoneStartUp);
	DefineUProperty(bool, bIsLargeTeamGame);
	DefineUProperty(EAthenaRespawnType, RespawnType);
	DefineUProperty(uint8, AirCraftBehavior);
	DefineUProperty(UAthenaAISettings*, AISettings);
	DefineUProperty(bool, bUseSameDirectionForOpposingAircraft);
};
