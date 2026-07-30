#include "pch.h"
#include "FortniteGame/Public/Missions/FortMission_RiftSpawners.h"

#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/AI/FortAIEncounterInfo.h"

void AFortMission_RiftSpawners::HandleAthenaGamePhaseChanged(uint8 GamePhase)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleAthenaGamePhaseChanged");

	if (!Func) {
		Log("AFortMission_RiftSpawners::HandleAthenaGamePhaseChanged: Failed to find function!");
		return;
	}

	return const_cast<AFortMission_RiftSpawners*>(this)->Call<void>(Func, GamePhase);
}

void AFortMission_RiftSpawners::HandleAthenaSafeZonePhaseChanged()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleAthenaSafeZonePhaseChanged");

	if (!Func) {
		Log("AFortMission_RiftSpawners::HandleAthenaSafeZonePhaseChanged: Failed to find function!");
		return;
	}

	return const_cast<AFortMission_RiftSpawners*>(this)->Call<void>(Func);
}

void AFortMission_RiftSpawners::HandleAISpawned(UFortAIEncounterInfo* Encounter, AFortAIPawn* SpawnedEnemy)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleAISpawned");

	if (!Func) {
		Log("AFortMission_RiftSpawners::HandleAISpawned: Failed to find function!");
		return;
	}

	return const_cast<AFortMission_RiftSpawners*>(this)->Call<void>(Func, Encounter, SpawnedEnemy);
}

void AFortMission_RiftSpawners::HandleRampStarted(UFortAIEncounterInfo* Encounter)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleRampStarted");

	if (!Func) {
		Log("AFortMission_RiftSpawners::HandleRampStarted: Failed to find function!");
		return;
	}

	return const_cast<AFortMission_RiftSpawners*>(this)->Call<void>(Func, Encounter);
}

void AFortMission_RiftSpawners::HandleEncounterAllBurstEnemiesSpawned(UFortAIEncounterInfo* Encounter)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("HandleEncounterAllBurstEnemiesSpawned");

	if (!Func) {
		Log("AFortMission_RiftSpawners::HandleEncounterAllBurstEnemiesSpawned: Failed to find function!");
		return;
	}

	return const_cast<AFortMission_RiftSpawners*>(this)->Call<void>(Func, Encounter);
}
