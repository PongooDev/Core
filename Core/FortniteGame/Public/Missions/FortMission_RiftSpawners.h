#pragma once
#include "pch.h"

#include "FortniteGame/Public/Missions/FortMission.h"
#include "FortniteGame/Public/Missions/FortRiftSpawnerData.h"

#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/AI/FortEncounterSettings.h"

class AController;
class AFortAIPawn;
class UFortAIEncounterInfo;
class UPrimitiveComponent;

class AFortMission_RiftSpawners : public AFortMission {
public:
	DefineUnrealClass(AFortMission_RiftSpawners);

	DefineUProperty(bool, bStartBasedOnAthenaGamePhases);
	DefineUProperty(uint8, StartingGamePhase);
	DefineUProperty(float, StartingDelay);
	DefineUProperty(bool, bUseAthenaSafeZonePhases);
	DefineUProperty(bool, bSynchronizeEncounterStartTimes);
	DefineUProperty(int32, MinEncounterIndex);
	DefineUProperty(int32, MaxEncounterIndex);
	DefineUProperty(FFortEncounterSettings, EncounterSettings);
	DefineUProperty(bool, bAllowRiftIntensification);
	DefineUProperty(FScalableFloat, MinDamageTriggerPercentage);
	DefineUProperty(FScalableFloat, MaxDamageTriggerPercentage);
	DefineUProperty(FGameplayTagQuery, RequiredRiftTagsQuery);
	DefineUProperty(float, SpawnerShutdownCheckInterval);
	DefineUProperty(float, BurstFallbackTime);
	DefineUProperty(TArray<FFortRiftSpawnerData>, ActiveRiftSpawnerData);
	DefineUProperty(int32, CurrentEncounterIndex);
	DefineUProperty(bool, bRiftSpawningInProgress);
	DefineUProperty(float, SyncedEncounterStartTime);
public:
	void HandleAthenaGamePhaseChanged(uint8 GamePhase);

	void HandleAthenaSafeZonePhaseChanged();

	void HandleAISpawned(UFortAIEncounterInfo* Encounter, AFortAIPawn* SpawnedEnemy);

	void HandleRampStarted(UFortAIEncounterInfo* Encounter);

	void HandleEncounterAllBurstEnemiesSpawned(UFortAIEncounterInfo* Encounter);
};
