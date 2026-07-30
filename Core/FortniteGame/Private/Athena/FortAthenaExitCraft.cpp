#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraft.h"
#include "FortniteGame/Public/Athena/ExitCraftInfo.h"

#include "Core/Public/StubCallsites.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Heist.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/FortSpecialActorReplicationInfo.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraftInfo.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraftSpawner.h"

void AFortAthenaExitCraftSpawner::SpawnExitCraft(AFortAthenaExitCraftSpawner* This)
{
	if (!This)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState ? World->GameState->Cast<AFortGameStateAthena>() : nullptr;

	UFortAthenaExitCraftInfo* Info = This->ExitCraftInfo;
	if (!Info) {
		Log("AFortAthenaExitCraftSpawner::SpawnExitCraft: spawner has no ExitCraftInfo!");
		return;
	}

	UClass* CraftClass = Info->ExitCaftClass.Get();
	if (!CraftClass) {
		Log("AFortAthenaExitCraftSpawner::SpawnExitCraft: ExitCaftClass is null on " + Info->GetName().ToString());
		return;
	}

	FVector SpawnLoc = This->K2_GetActorLocation();
	FRotator SpawnRot = This->K2_GetActorRotation();

	if (Info->_HasExitCraftInfo())
		SpawnLoc.Z += Info->ExitCraftInfo.ExitCraftZOffset.Evaluate(0);

	AFortAthenaExitCraft* Craft = (AFortAthenaExitCraft*)World->SpawnActorUnfinished(CraftClass, SpawnLoc, SpawnRot);
	if (!Craft) {
		Log("AFortAthenaExitCraftSpawner::SpawnExitCraft: failed to spawn " + CraftClass->GetName().ToString());
		return;
	}

	if (!Craft->ExitCraftInfo)
		Craft->ExitCraftInfo = Info;

	World->FinishSpawnActor(Craft, SpawnLoc, SpawnRot);

	AFortSpecialActorReplicationInfo* SpecialActorData = GameState ? GameState->SpecialActorData : nullptr;
	if (SpecialActorData && Info->_HasSpecialActorCraftTag()
		&& Info->_HasCraftMinimapIconBrush() && Info->_HasCraftCompassIconBrush()) {
		FName CraftID = SpecialActorData->RegisterSpecialActor(
			Craft,
			Info->SpecialActorCraftTag,
			Info->CraftMinimapIconBrush, Info->CraftMinimapIconScale,
			Info->CraftCompassIconBrush, Info->CraftCompassIconScale
		);

		if (!CraftID.IsNone()) {
			if (Craft->_HasCraftSpecialActorID())
				Craft->CraftSpecialActorID = CraftID;

			if (This->_HasSpawnerSpecialActorID() && !This->SpawnerSpecialActorID.IsNone())
				SpecialActorData->RemoveSpecialActor(This->SpawnerSpecialActorID);
		}
	}

	if (GameState) {
		for (AFortAthenaMutator* Mutator : GameState->GameplayMutators) {
			AFortAthenaMutator_Heist* Heist = Mutator ? Mutator->Cast<AFortAthenaMutator_Heist>() : nullptr;
			if (!Heist)
				continue;

			for (int32 i = 0; i < Heist->SpawnedExitCraftList.Num(); i++) {
				FHeistExitCraftData& Entry = Heist->SpawnedExitCraftList.GetWithSize(i, FHeistExitCraftData::GetSize());

				if (Entry.ExitCraftSpawner == This && !Entry.SpawnedExitCraft) {
					Entry.SpawnedExitCraft = Craft;
					break;
				}
			}

			Heist->OnExitCraftSpawned(Craft, This);
		}
	}

	This->K2_DestroyActor();
}

void AFortAthenaExitCraftSpawner::BeginPlay(AFortAthenaExitCraftSpawner* This)
{
	BeginPlayOG(This);

	if (!This)
		return;

	This->StartExitCraftSpawnTimer();
}

void AFortAthenaExitCraftSpawner::Hook()
{
	if (Finder::FindAActor_BeginPlayVFT()) {
		HookEveryVTableIdx(
			AFortAthenaExitCraftSpawner::StaticClass(),
			Finder::FindAActor_BeginPlayVFT(),
			BeginPlay,
			(LPVOID*)&BeginPlayOG
		);
	}

	uintptr_t Binder = StubCallsites::FromReflection("Function /Script/FortniteGame.FortAthenaExitCraftSpawner.StartExitCraftSpawnTimer");
	uintptr_t Stub = StubCallsites::ResolveEmptyStub(Binder);

	StubCallsites::PatchBound("AFortAthenaExitCraftSpawner::SpawnExitCraft", Stub, SpawnExitCraft, {
		{ "AFortAthenaExitCraftSpawner::StartExitCraftSpawnTimer", {
			[=] { return Binder; } } },
		});
}
