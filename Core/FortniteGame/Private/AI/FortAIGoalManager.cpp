#include "pch.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"

#include "FortniteGame/Public/FortGameModeZone.h"
#include "Core/Public/StubCallsites.h"

AFortAIGoalManager* AFortAIGoalManager::GetCurrent(UObject* WorldContextObject) {
	if (!WorldContextObject)
		return nullptr;

	UWorld* World = WorldContextObject->GetWorld();
	if (!World)
		return nullptr;

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode)
		return nullptr;

	return GameMode->AIGoalManager;
}

void AFortAIGoalManager::Hook() {
	uintptr_t Stub = StubCallsites::ResolveStub(
		StubCallsites::FromString(L"OnReceiveGoalQueryResult received by AFortAIController when no Goal Manager exists!"));

	StubCallsites::Patch("AFortAIGoalManager::GetCurrent", Stub, GetCurrent, {
		{ "AFortMission::CreateEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.CreateEncounterAssignment") } },

		{ "AFortMission::AddGoalToEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.AddGoalToEncounterAssignment") } },

		{ "AFortMission::AddGoalsToEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.AddGoalsToEncounterAssignment") } },

		{ "AFortMission::FindGoalLocationsForEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.FindGoalLocationsForEncounterAssignment") } },

		{ "AFortMission::RemoveGoalFromEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.RemoveGoalFromEncounterAssignment") } },

		{ "AFortMission::RemoveGoalsFromEncounterAssignment", {
			StubCallsites::ByReflection("Function /Script/FortniteGame.FortMission.RemoveGoalsFromEncounterAssignment") } },

		{ "UFortQueryGenerator::GenerateItems", {
			StubCallsites::BySignature("48 89 5C 24 ? 48 89 74 24 ? 57 48 83 EC 20 48 8B DA 48 8B F1 48 85 D2 74 ? E8 ? ? ? ? 48 8B D0 48 8B CB E8 ? ? ? ? 48 8B F8 48 85 C0 74") } },

		{ "UFortQueryGenerator::GenerateItems", {
			StubCallsites::BySignature("48 89 5C 24 ? 48 89 6C 24 ? 56 48 83 EC 20 48 8B F1 48 8B DA") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 30 01 00 00") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC A0 00 00 00 48 8B F1"), // 1.9
			StubCallsites::BySignature("40 55 56 57 41 54 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC B0 00 00 00 48 8B F1") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 5C 24 ? 55 56 57 41 56 41 57 48 8D 6C 24 ? 48 81 EC D0 00 00 00 48 8D 7A") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC 90 00 00 00") } },

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 56 57 41 55 41 57 48 8D 6C 24 ? 48 81 EC 10 01 00 00 4C 8B F9"), // 1.9
			StubCallsites::BySignature("40 55 56 57 41 55 41 57 48 8D 6C 24 ? 48 81 EC 20 01 00 00 4C 8B F9") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 50 01 00 00"), // 1.9
			StubCallsites::BySignature("48 89 54 24 ? 48 89 4C 24 ? 55 53 56 57 41 56 48 8D 6C 24 ? 48 81 EC 60 01 00 00") } }, // 3.6

		{ "UFortQueryTest::RunTest", {
			StubCallsites::BySignature("40 55 53 56 57 41 55 41 56 41 57 48 8D 6C 24 ? 48 81 EC F0 00 00 00 48 8D 7A") } },
	}, false);

	Log("AFortAIGoalManager Hooked");
}
