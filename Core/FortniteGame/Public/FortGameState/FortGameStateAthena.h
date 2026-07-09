#pragma once
#include "pch.h"

#include "FortGameStateZone.h"
#include "FortniteGame/Public/FortPlaylist/PlaylistPropertyArray.h"

class AFortAthenaMapInfo;
class APlayerState;
class UAthenaBattleBusItemDefinition;
class AFortAthenaPlaylistBase;
class UFortPlaylistAthena;

class AFortGameStateAthena : public AFortGameStateZone {
public:
	DefineUnrealClass(AFortGameStateAthena);

	DefineUProperty(AFortAthenaMapInfo*, MapInfo);

	DefineUProperty(FString, WinningPlayerName);
	DefineUProperty(APlayerState*, WinningPlayerState);
	DefineUProperty(int32, WinningTeam);
	DefineUProperty(int32, TotalPlayers);
	DefineUProperty(int32, PlayersLeft);
	DefineUProperty(int32, FiftyFiftyTeam1PlayersLeft);
	DefineUProperty(int32, FiftyFiftyTeam2PlayersLeft);
	DefineUProperty(int32, TeamsLeft);
	DefineUProperty(FPlaylistPropertyArray, CurrentPlaylistInfo);
	DefineUProperty(UFortPlaylistAthena*, CurrentPlaylistData);
	DefineUProperty(int32, CurrentPlaylistId);
	DefineUProperty(UAthenaBattleBusItemDefinition*, DefaultBattleBus);
	DefineUProperty(AFortAthenaPlaylistBase*, FortAthenaPlaylistCDO);
	DefineUProperty(uint8, AirCraftBehavior);
	DefineUProperty(uint8, CachedSafeZoneStartUp);
	DefineUProperty(bool, bIsLargeTeamGame);
public:
	void OnRep_CurrentPlaylistId();

	void OnPlaylistDataLoadCompleted();
	void UpdatePlaylistDependentData();

	void SetCurrentPlaylistId(int InPlaylistId);

	static inline void (*ApplyHomebaseEffectsOnPlayerSetupOG)(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);
	static void ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);

	void OnRep_WinningPlayerName();

	void OnRep_WinningTeam();

	void OnRep_WinningPlayerList();

	void OnRep_WinningPlayerState();

	void OnRep_CurrentPlaylistData();

	void OnRep_CurrentPlaylistInfo();

	static void Hook();
};