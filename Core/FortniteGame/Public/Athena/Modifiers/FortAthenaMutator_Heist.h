#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortPlayerPawnAthena;
class AFortPlayerStateAthena;
class AFortAthenaExitCraft;
class AFortAthenaExitCraftSpawner;
class UFortAthenaExitCraftInfo;
class USoundCue;

struct FHeistExitCraftSpawnData {
public:
	DefineUnrealStruct(FHeistExitCraftSpawnData);

	DefineStructProperty(FScalableFloat, SpawnDirection);
	DefineStructProperty(FScalableFloat, SpawnDirectionDeviation);
	DefineStructProperty(FScalableFloat, MinSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, MaxSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, SpawnDelayTime);
	DefineStructProperty(FScalableFloat, SafeZonePhaseWhenToSpawn);
	DefineStructProperty(FScalableFloat, SafeZonePhaseWhereToSpawn);
public:
	uint8 Padding[0x118];
};

struct FHeistBlingDropSpawnData {
public:
	DefineUnrealStruct(FHeistBlingDropSpawnData);

	DefineStructProperty(FScalableFloat, SpawnDirection);
	DefineStructProperty(FScalableFloat, SpawnDirectionDeviation);
	DefineStructProperty(FScalableFloat, MinSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, MaxSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, SafeZonePhaseWhereToSpawn);
public:
	uint8 Padding[0xC8];
};

struct FHeistExitCraftData {
public:
	DefineUnrealStruct(FHeistExitCraftData);

	DefineStructProperty(AFortAthenaExitCraftSpawner*, ExitCraftSpawner);
	DefineStructProperty(AFortAthenaExitCraft*, SpawnedExitCraft);
	DefineStructProperty(TArray<AFortPlayerPawnAthena*>, DepartedPawns);
	DefineStructProperty(float, SpawnTime);
	DefineStructProperty(bool, bIsUsed);
	DefineStructProperty(bool, bHasDeparted);
public:
	uint8 Padding[0x30];
};

struct FHeistTeamHoldingJewelInfo {
public:
	DefineUnrealStruct(FHeistTeamHoldingJewelInfo);

	DefineStructProperty(int32, JewelsHeld);
	DefineStructProperty(float, TimeStartedHoldingJewel);
	DefineStructProperty(float, AccumulatedTotalTime);
public:
	uint8 Padding[0xC];
};

using FHeistJewelHeldByTeamMap = TMap<uint8, FHeistTeamHoldingJewelInfo>;

class AFortAthenaMutator_Heist : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Heist);

	DefineUProperty(UFortAthenaExitCraftInfo*, ExitCraftInfo);
	DefineUProperty(TArray<FHeistExitCraftSpawnData>, HeistExitCraftSpawnData);
	DefineUProperty(TArray<FHeistBlingDropSpawnData>, HeistBlingDropSpawnData);
	DefineUProperty(USoundCue*, HeistVictorySoundCue);
	DefineUProperty(FScalableFloat, SpawnExitCraftInitialDelay);
	DefineUProperty(FScalableFloat, BlingCarryingEnemiesVisibleTime);
	DefineUProperty(TArray<FHeistExitCraftData>, SpawnedExitCraftList);
	DefineUProperty(bool, bCurrExitCraftDeparted);
	DefineUProperty(TArray<AFortPlayerPawnAthena*>, ExitCraftOverlappingPawns);
	DefineUProperty(float, SpawnExitCraftTime);
	DefineUProperty(int32, CurrExitCraftIndexToSpawn);
	DefineUProperty(float, SupplyDropStartingAngle);
	DefineUProperty(float, ExitCraftStartingAngle);
	DefineUProperty(int32, CurrBlingSupplyDropIndexHandled);
	DefineUProperty(TArray<int32>, RemainingExitCraftSpawnIndexes);
	DefineUProperty(TArray<int32>, RemainingSupplyDropSpawnIndexes);
	DefineUProperty(int32, NumUnspawnedExitCrafts);
	DefineUProperty(int32, NumSpawnedExitCrafts);
	DefineUProperty(int32, NumDepartedExitCrafts);
	DefineUProperty(TArray<int32>, NumPlayersAliveOnDeparture);
	DefineUProperty(int32, JewelsLostToStorm);
	DefineUProperty(int32, SafesLostToStorm);
	DefineUProperty(FHeistJewelHeldByTeamMap, JewelHeldByTeamMap);
	DefineUProperty(FGameplayTag, BlingTrackingCategoryTag);
	DefineUProperty(FGameplayTag, SafeSupplyDropTag);
	DefineUProperty(FGameplayTag, BlingItemTag);
	DefineUProperty(FGameplayTag, CarryingBlingItemTag);
	DefineUProperty(FGameplayTag, ExitCraftTag);
	DefineUProperty(FGameplayTag, BlingLootItemTag);
public:
	void OnGamePhaseChanged(uint8 GamePhase);

	void OnGamePhaseStepChanged(uint8 GamePhaseStep);

	void OnExitCraftSpawned(AFortAthenaExitCraft* ExitCraft, AFortAthenaExitCraftSpawner* ExitCraftSpawner);

	void OnExitCraftIsDestroying(AFortAthenaExitCraft* ExitCraft);

	void SendPlayerAcquiredMessage(AFortPlayerStateAthena* CarryingPlayerState);

	void OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2);
	static inline void (*execOnMutatorGameplayEventOG)(AFortAthenaMutator_Heist* Context, FFrame& Stack);
	static void execOnMutatorGameplayEvent(AFortAthenaMutator_Heist* Context, FFrame& Stack);

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.FortAthenaMutator_Heist.OnMutatorGameplayEvent", execOnMutatorGameplayEvent, execOnMutatorGameplayEventOG);

		Log("AFortAthenaMutator_Heist Hooked!");
	}
};
