#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeZone.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateZone.h"
#include "FortniteGame/Public/FortManager/FortMissionManager.h"
#include "FortniteGame/Public/Kismet/FortMissionLibrary.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"

void AFortGameModeZone::HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer) {
	Log("HandleStartingNewPlayer Called!");

	if (Version::Fortnite_Version <= 3.6 && Version::Fortnite_Version >= 1.8) {
		if (NewPlayer->PlayerCameraManager) {
			NewPlayer->PlayerCameraManager->ViewRollMin = 0.0f;
			NewPlayer->PlayerCameraManager->ViewRollMax = 0.0f;
		}
	}

	// genuinely fuck yourself game, this is the most retarded shit i've ever seen in my life
	if (NewPlayer->_HasQuickBars() && !NewPlayer->QuickBars)
	{
		NewPlayer->SpawnQuickBars();
		NewPlayer->SetupQuickBars();
	}

	return HandleStartingNewPlayerOG(This, NewPlayer);
}

void AFortGameModeZone::CreateAIDirector() {
	if (Finder::FindAFortGameModeZone_CreateAIDirectorVFT()) {
		void (*&CreateAIDirectorInternal)(AFortGameModeZone * This) = decltype(CreateAIDirectorInternal)(VTable[Finder::FindAFortGameModeZone_CreateAIDirectorVFT()]);
		CreateAIDirectorInternal(this);
	}
	if (!AIDirector) {
		AIDirector = (AFortAIDirector*)GetWorld()->SpawnActor(AFortAIDirector::StaticClass(), FVector(), FRotator(), this);
		if (AIDirector) {
			Log("AFortGameModeZone::CreateAIDirector: Created AIDirector: " + AIDirector->GetName().ToString());
			AIDirector->Activate();
		}
		else {
			Log("AFortGameModeZone::CreateAIDirector: Failed to create AIDirector");
		}
	}
}

void AFortGameModeZone::CreateAIGoalManager() {
	if (Finder::FindAFortGameModeZone_CreateAIGoalManagerVFT()) {
		void (*&CreateAIGoalManagerInternal)(AFortGameModeZone * This) = decltype(CreateAIGoalManagerInternal)(VTable[Finder::FindAFortGameModeZone_CreateAIGoalManagerVFT()]);
		CreateAIGoalManagerInternal(this);
	}
	if (!AIGoalManager) {
		AIGoalManager = (AFortAIGoalManager*)GetWorld()->SpawnActor(AFortAIGoalManager::StaticClass(), FVector(), FRotator(), this);
		if (AIGoalManager) {
			Log("AFortGameModeZone::CreateAIGoalManager: Created AIGoalManager: " + AIGoalManager->GetName().ToString());
		}
		else {
			Log("AFortGameModeZone::CreateAIGoalManager: Failed to create AIGoalManager");
		}
	}
}

void AFortGameModeZone::FinishWorldInitialization(AFortGameModeZone* This, AFortWorldManager* WorldManager) {
	AFortGameMode::FinishWorldInitialization(This, WorldManager);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameModeZone::FinishWorldInitialization: World is null!");
		return;
	}

	AFortGameStateZone* FortGameStateZone = This->GameState->Cast<AFortGameStateZone>();
	if (!FortGameStateZone) {
		Log("AFortGameModeZone::FinishWorldInitialization: GameState is null or not AFortGameStateZone");
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = This->Cast<AFortGameModeAthena>();
	
	This->CreateAIDirector();
	This->CreateAIGoalManager();

	FortGameStateZone->GameDifficulty = 10.f;
	FortGameStateZone->OnRep_GameDifficulty();

	if (!FortGameModeAthena && FortGameStateZone->MissionManager) {
		UFortMissionInfo* MissionInfo = StaticLoadObject<UFortMissionInfo>("/Game/Missions/Primary/EvacuateTheSurvivors/EvacuteTheSurvivors.EvacuteTheSurvivors");

		if (MissionInfo) {
			Log("AFortGameModeZone::FinishWorldInitialization: Loading mission " + MissionInfo->GetName().ToString());
			UFortMissionLibrary::LoadMission(World, MissionInfo);
			Log("AFortGameModeZone::FinishWorldInitialization: Loaded mission " + MissionInfo->GetName().ToString());
		}
	}

	TArray<AActor*> ItemCollectors;
	UGameplayStatics::GetAllActorsOfClass(World, ABuildingItemCollectorActor::StaticClass(), &ItemCollectors);
	for (AActor* ItemCollectorActor : ItemCollectors) {
		ABuildingItemCollectorActor* ItemCollector = ItemCollectorActor->Cast<ABuildingItemCollectorActor>();
		if (ItemCollector) {
			if (!ItemCollector->Setup()) {
				Log("AFortGameModeZone::FinishWorldInitialization: Failed to setup ItemCollector: " + ItemCollector->GetName().ToString());
			}
		}
	}
}

APawn* AFortGameModeZone::SpawnDefaultPawnFor(AFortGameModeZone* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameMode::SpawnDefaultPawnFor(This, NewPlayer, StartSpot);

	Log("AFortGameModeZone::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

void AFortGameModeZone::AddInactivePlayerHK(AFortGameModeZone* This, APlayerState* PlayerState, APlayerController* PC) {
	return AGameMode::AddInactivePlayerOG(This, PlayerState, PC);
}

void AFortGameModeZone::Hook() {
	HookEveryVTable(AFortGameModeZone::StaticClass(),
		AGameModeBase::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.HandleStartingNewPlayer"),
		HandleStartingNewPlayer,
		(LPVOID*)&HandleStartingNewPlayerOG
	);

	HookVTableIdx(
		AFortGameModeZone::GetDefaultObj(),
		Finder::FindAFortGameMode_FinishWorldInitializationVFT(),
		FinishWorldInitialization,
		(LPVOID*)&FinishWorldInitializationOG
	);

	HookVTable(
		AFortGameModeZone::GetDefaultObj(),
		AFortGameModeZone::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"),
		SpawnDefaultPawnFor,
		(LPVOID*)&SpawnDefaultPawnForOG
	);

	HookVTableIdx(
		AFortGameModeZone::GetDefaultObj(),
		Finder::FindAGameMode_AddInactivePlayerVFT(),
		AddInactivePlayerHK
	);

	Log("Hooked AFortGameModeZone");
}