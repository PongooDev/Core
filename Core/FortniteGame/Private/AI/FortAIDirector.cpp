#include "pch.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"

#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Building/BuildingActor.h"
#include "Core/Public/StubCallsites.h"

void AFortAIDirector::Activate()
{
	if (bNightActive)
		return;

	NightCount++;

	const bool bNoActiveEncounters = ActiveEncounters.Num() == 0;

	bNightActive = true;

	if (bNoActiveEncounters) {
		UClass* EncounterClass = DefaultNightEncounter.Class;
		if (EncounterClass && EncounterClass->IsSubclassOf(UFortAIEncounterInfo::StaticClass())) {
			ActiveDefaultEncounter = StartEncounterWithoutObjective(DefaultNightEncounter);
		}
	}

	for (int32 i = 0; i < ActiveEncounters.Num(); ++i)
	{
		if (UFortAIEncounterInfo* Encounter = ActiveEncounters[i])
		{
			Encounter->NotifyNightStarted();
		}
	}
}

UFortAIEncounterInfo* AFortAIDirector::StartEncounterWithoutObjective(TSubclassOf<UFortAIEncounterInfo> EncounterTemplate)
{
	UFortAIEncounterInfo* (*StartEncounterWithoutObjectiveInternal)(AFortAIDirector*, TSubclassOf<UFortAIEncounterInfo>) =
		decltype(StartEncounterWithoutObjectiveInternal)(ImageBase + Finder::FindAFortAIDirector_StartEncounterWithoutObjective());

	return StartEncounterWithoutObjectiveInternal(this, EncounterTemplate);
}

void AFortAIDirector::execActivate(AFortAIDirector* Context, FFrame& Stack) {
	Stack.IncrementCode();
	
	Context->Activate();
}

AFortAIDirector* AFortAIDirector::GetCurrent(UObject* WorldContextObject) {
	//Log("AFortAIDirector::GetCurrent");
	if (!WorldContextObject) {
		Log("AFortAIDirector::GetCurrent: Failed due to no WorldContextObject!");
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("AFortAIDirector::GetCurrent: Failed due to no World!");
		return nullptr;
	}

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode) {
		Log("AFortAIDirector::GetCurrent: Failed because GameMode is either null or not an AFortGameModeZone!");
		return nullptr;
	}

	return GameMode->AIDirector;
}

void AFortAIDirector::Hook() {
	ExecHook("Function /Script/FortniteGame.FortAIDirector.Activate", execActivate);

	{
		uintptr_t EncounterSequence = StubCallsites::FromString(L"Could not create encounter sequence with given tags: %s, generated sequence not found");
		uintptr_t Stub = StubCallsites::ResolveStub(EncounterSequence);

		StubCallsites::Patch("AFortAIDirector::GetCurrent", Stub, GetCurrent, {
			{ "AFortMission::CreateEncounterSequence", {
				[=] { return EncounterSequence; } } },

			{ "ABuildingActor::HandleDamaged", {
				StubCallsites::ByVTable(ABuildingActor::StaticClass(), Finder::FindABuildingActor_HandleDamagedVFT()) } },

			{ "AFortMission::StartMissionAIEncounter", {
				StubCallsites::ByString(L"StartMissionAIEncounter: No AI Director!") } },

			{ "AFortMission::StartMissionAIEncounterFromGeneratedProfile", {
				StubCallsites::ByString(L"StartMissionAIEncounterFromGeneratedProfile: No AI Director!") } },

			{ "AFortMission::StopEncounterSequence", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.StopEncounterSequence") } },

			{ "AFortMission::StopMissionAIEncounter", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.StopMissionAIEncounter") } },

			{ "ABuildingTrap::FinishTrigger", {
				StubCallsites::ByOffset(Finder::FindABuildingTrap_FinishTrigger()) } },

			{ "AFortGameMode::GetFriendlyActors", {
				StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 57 41 56 41 57 48 83 EC ? 41 8B 40") } },

			{ "UFortBTService_UpdateBotMissionGoal::TickNode", {
				StubCallsites::BySignature("48 8B C4 48 89 50 ? 55 48 8D 68 ? 48 81 EC ? ? ? ? 48 89 58 ? 48 89 70 ? 48 89 78 ? 4C 89 60 ? 4C 89 68 ? 4C 8B EA 4C 89 78") } },

			{ "FUndermineHelpers::AreBuildingsInRange", {
				StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F 29 74 24 ? 48 8B D9 48 8B 89 C8 05 00 00"),
				StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 0F 29 74 24 ? 48 8B D9") } },

			{ "AFortPlayerController::CreateAIDirectorDataManager", {
				StubCallsites::ByXref("48 8B 81 ? ? ? ? 48 85 C0 74 ? 48 8B 40 ? ? ? ? 48 8B C2") } },

			{ "AFortUIZone::InitUtilitiesGraph", {
				StubCallsites::BySignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B 99 88 03 00 00 48 8B F9 48 85 DB 74"),
				StubCallsites::BySignature("48 89 5C 24 ? 57 48 83 EC ? 48 8B 99 ? ? ? ? 48 8B F9 48 85 DB 74 ? E8 ? ? ? ? 48 8B 53 ? 4C 8D 80 ? ? ? ? 49 63 40 ? 3B 82 ? ? ? ? 7F ? 48 8B C8 48 8B 82 ? ? ? ? ? ? ? ? 74 ? 33 DB 48 83 BF ? ? ? ? 00") } },

			{ "UFortCheatManager::CycleCurrentEncounterToDebug", {
				StubCallsites::ByString(L"No active encounters to debug."),
				StubCallsites::ByString(L"Now debugging encounter with index: %i of type: %s") } },

			{ "AFortPlayerControllerZone::UpdateNearbyEncounters", {
				StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 48 89 74 24 ? 57 48 83 EC ? 48 8B F9 48 8B CA E8") } },

			{ "UFortQueryTest_HasNearbyEncounterGoals::RunTest", {
				StubCallsites::BySignature("40 55 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC 98 00 00 00 4C 8B F1 4C 8B FA 48 8B 4A ? E8"),
				StubCallsites::BySignature("40 55 41 54 41 56 41 57 48 8D 6C 24 ? 48 81 EC ? ? ? ? 4C 8B F1 4C 8B FA 48 8B 4A ? E8 ? ? ? ? 4C 8B E0 48 85 C0 0F 84 ? ? ? ? 48 89 9C 24 ? ? ? ? 49 8D 4F ? 48 89 B4 24 ? ? ? ? 48 89 BC 24 ? ? ? ? 0F 29 B4 24 ? ? ? ? 0F 29 BC 24 ? ? ? ? 44 0F 29 44 24") } },

			{ "UFortCheatManager::ClearEncounterSimulatedNumberOfPlayers", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ClearEncounterSimulatedNumberOfPlayers") } },

			{ "UFortCheatManager::ClearEncounterSpawnPointsCap", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ClearEncounterSpawnPointsCap") } },

			{ "UFortCheatManager::ClearEncounterSpawnPointsCurve", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ClearEncounterSpawnPointsCurve") } },

			{ "UFortCheatManager::DisableAI", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.DisableAI") } },

			{ "UFortCheatManager::EncounterCurrentUtilities", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.EncounterCurrentUtilities") } },

			{ "UFortCheatManager::EncounterInitialUtilities", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.EncounterInitialUtilities") } },

			{ "UFortCheatManager::EncounterSetDifficultyLevel", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.EncounterSetDifficultyLevel") } },

			{ "UFortCheatManager::EncounterSpawnProbabilities", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.EncounterSpawnProbabilities") } },

			{ "UFortCheatManager::EncounterTopUtilityPercentages", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.EncounterTopUtilityPercentages") } },

			{ "UFortCheatManager::ListActiveEncounters", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ListActiveEncounters") } },

			{ "UFortCheatManager::ResetEncounterWave", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ResetEncounterWave") } },

			{ "UFortCheatManager::ResetMaxAITracking", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ResetMaxAITracking") } },

			{ "UFortCheatManager::SetCurrentEncounterToDebug", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetCurrentEncounterToDebug") } },

			{ "UFortCheatManager::SetEncounterDirections", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetEncounterDirections") } },

			{ "UFortCheatManager::SetEncounterSimulatedNumberOfPlayers", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetEncounterSimulatedNumberOfPlayers") } },

			{ "UFortCheatManager::SetEncounterSpawnPointsCap", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetEncounterSpawnPointsCap") } },

			{ "UFortCheatManager::SetEncounterSpawnPointsCurve", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetEncounterSpawnPointsCurve") } },

			{ "UFortCheatManager::SetEncounterTopUtilityPercentages", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.SetEncounterTopUtilityPercentages") } },

			{ "UFortCheatManager::ToggleAISpawnCap", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ToggleAISpawnCap") } },

			{ "UFortCheatManager::ToggleAISpawning", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ToggleAISpawning") } },

			{ "UFortCheatManager::ToggleEncounterEQSDebug", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ToggleEncounterEQSDebug") } },

			{ "UFortCheatManager::ToggleEncounterModifierTags", {
				StubCallsites::ByReflection("Function /Script/FortniteGame.FortCheatManager.ToggleEncounterModifierTags") } },
			
		}, false);
	}

	Log("AFortAIDirector Hooked");
}
