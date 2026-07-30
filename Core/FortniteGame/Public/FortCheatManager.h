#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/CheatManager.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Core/Public/Utils.h"

class AFortPlayerController;

class UFortCheatManager : public UCheatManager {
public:
	DefineUnrealClass(UFortCheatManager);
public:
	void AddAllScores(int32 Amount);

	void AddBuildingScore(int32 Amount);

	void AddCombatScore(int32 Amount);

	void AddUtilityScore(int32 Amount);

	void AddKillFeedMessage();
public:
	AFortPlayerController* GetPlayerController() const;

	void Help(FCommandParser& Parser);
	void GiveItem(FCommandParser& Parser);
	void ForceGiveItem(FCommandParser& Parser);
	void SpawnPickup(FCommandParser& Parser);
	void SetLoadedAmmo(FCommandParser& Parser);
	void GiveAmmo(FCommandParser& Parser);
	void DumpInventory(FCommandParser& Parser);
	void SetHealth(FCommandParser& Parser);
	void SetShield(FCommandParser& Parser);
	void SetMaxHealth(FCommandParser& Parser);
	void SetMaxShield(FCommandParser& Parser);
	void SpawnActor(FCommandParser& Parser);
	void ClearEquippedItem(FCommandParser& Parser);
	void GetWeaponStats(FCommandParser& Parser);
	void DestroyTarget(FCommandParser& Parser);
	void DumpActorsWithClass(FCommandParser& Parser);
	void TeleportToLocation(FCommandParser& Parser);
	void TeleportToActor(FCommandParser& Parser);
	void StartEvent(FCommandParser& Parser);
	void DumpCurrentLocation(FCommandParser& Parser);
	void SpawnQuickBars(FCommandParser& Parser);
	void DestroyQuickBars(FCommandParser& Parser);
	void DumpQuickBars(FCommandParser& Parser);
	void ServerExecuteInventoryItem(FCommandParser& Parser);
	void PossessPawnByIndex(FCommandParser& Parser);
	void PossessPawnByName(FCommandParser& Parser);
	void DumpAllPawns(FCommandParser& Parser);
	void SetKillScore(FCommandParser& Parser);
	void LootRain(FCommandParser& Parser);
	void SetGameSpeed(FCommandParser& Parser);
	void DespawnAllBots(FCommandParser& Parser);
	void TeleportAllToMe(FCommandParser& Parser);
	void SwapPlaces(FCommandParser& Parser);
	void LaunchPawn(FCommandParser& Parser);
	void SetScale(FCommandParser& Parser);
	void ScalePawn(FCommandParser& Parser);
	void Goto(FCommandParser& Parser);
	void DestroyBuildings(FCommandParser& Parser);
	void EmoteAll(FCommandParser& Parser);
	void EmoteAllSpecific(FCommandParser& Parser);
	void EmotePlayerByName(FCommandParser& Parser);
	void TogglePersonalVehicle(FCommandParser& Parser);
	void DumpAircrafts(FCommandParser& Parser);
	void ActivateRifts(FCommandParser& Parser);
	void DumpEncounters(FCommandParser& Parser);
	void StartEncounter(FCommandParser& Parser);
	void SetSpawnExitCraftTime(FCommandParser& Parser);
	void DumpExitCraftTimer(FCommandParser& Parser);
	void UpdateGamePhaseStep(FCommandParser& Parser);
	void DumpGameState(FCommandParser& Parser);
	void SpawnExitCraft(FCommandParser& Parser);
};