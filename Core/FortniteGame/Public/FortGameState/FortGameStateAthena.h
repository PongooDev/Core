#pragma once
#include "pch.h"

#include "FortGameStateZone.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"
#include "FortniteGame/Public/FortPlaylist/PlaylistPropertyArray.h"
#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/Athena/AircraftFlightInfo.h"
#include "FortniteGame/Public/FortMutator/MutatorObjectData.h"

class AFortAthenaMapInfo;
class APlayerState;
class UAthenaBattleBusItemDefinition;
class AFortAthenaPlaylistBase;
class UFortPlaylistAthena;
class AFortAthenaAircraft;
class UCurveTable;
class AFortPlayerState;
class AFortSafeZoneIndicator;
class AFortAthenaMutator;
class AFortAthenaMutator_AI;
class UFortMutatorListComponent;
class AFortSpecialActorReplicationInfo;

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
	DefineUProperty(EAirCraftBehavior, AirCraftBehavior);
	DefineUProperty(bool, bUseSameDirectionForOpposingAircraft);
	DefineUProperty(TArray<AFortAthenaAircraft*>, Aircrafts);
	DefineUProperty(TArray<AFortAthenaMutator*>, GameplayMutators);
	DefineUProperty(FGameplayMutatorEventData, MutatorEventData);
	DefineUProperty(FGameplayMutatorObjectDataArray, MutatorObjectDataArray);
	DefineUProperty(int32, MutatorGenericInt_0);
	DefineUProperty(int32, MutatorGenericInt_1);
	DefineUProperty(int32, MutatorGenericInt_2);
	DefineUProperty(AFortAthenaMutator_AI*, GameplayMutator_AI);
	DefineUProperty(UFortMutatorListComponent*, MutatorListComponent);
	DefineUProperty(AFortSpecialActorReplicationInfo*, SpecialActorData);
	DefineUProperty(uint8, GamePhase);
	DefineUProperty(uint8, SafeZonePhase);
	DefineUProperty(uint8, GamePhaseStep);
	DefineUProperty(float, WarmupCountdownStartTime);
	DefineUProperty(float, WarmupCountdownEndTime);
	DefineUProperty(float, SafeZonesStartTime);
	DefineUProperty(float, EndGameKickPlayerTime);
	DefineUProperty(float, GamePhaseStepTimeRemaining);
	DefineUProperty(bool, bIsInCountdown);
	DefineUProperty(bool, bIsInFinalCountdown);
	DefineBitfieldUProperty(bAircraftIsLocked);
	DefineUProperty(FMulticastScriptDelegate, GamePhaseStepChanged);
	DefineUProperty(uint8, CachedSafeZoneStartUp);
	DefineUProperty(bool, bIsLargeTeamGame);
	DefineUProperty(bool, bStormReachedFinalPosition);
	DefineUProperty(bool, bDrawSafeZoneFinalPosIconEnabled);
	DefineUProperty(bool, bPlaylistDataIsLoaded);
	DefineUProperty(bool, bPlaylistDataIsActivelyLoading);
	DefineUProperty(FAircraftFlightInfo, FlightPathMidLine);
	DefineUProperty(AFortSafeZoneIndicator*, SafeZoneIndicator);
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

	void OnRep_Aircraft();

	UDataTable* GetLootTierData();
	UDataTable* GetLootPackages();

	UCurveTable* GetGameData();

	UFortPlaylistAthena* GetPlaylist();

	void LoadCurrentPlaylistData();

	void InitializePlaylistDataPreDataLoad();

	float GetServerWorldTimeSeconds();

	uint8 GetGamePhaseStep(float& OutTimeRemaining);
	void UpdateGamePhaseStep();

	FVector GetSafeZoneCenter();

	static inline void (*TickOG)(AFortGameStateAthena* This, float DeltaSeconds);
	static void Tick(AFortGameStateAthena* This, float DeltaSeconds);

	static void Hook();
};