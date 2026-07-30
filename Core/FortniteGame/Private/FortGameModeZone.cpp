#include "pch.h"
#include "FortniteGame/Public/FortGameModeZone.h"

#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Player/FortPlayerControllerZone.h"
#include "FortniteGame/Public/Player/FortPlayerStateZone.h"
#include "FortniteGame/Public/Abilities/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameStateZone.h"
#include "FortniteGame/Public/Missions/FortMissionManager.h"
#include "FortniteGame/Public/Missions/FortMissionLibrary.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/Building/BuildingFoundation.h"
#include "FortniteGame/Public/Building/BGAConsumableSpawner.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/FortEnums.h"

void AFortGameModeZone::HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer) {
	Log("HandleStartingNewPlayer Called!");
	if (Version::Fortnite_Version <= 6.0 && Version::Fortnite_Version >= 1.8) {
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

	FortGameStateZone->GameDifficulty = 10.f;
	FortGameStateZone->OnRep_GameDifficulty();

	if (This->AssociatedSubGame == ESubGame::GetCampaign() && FortGameStateZone->MissionManager) {
		UFortMissionInfo* MissionInfo = StaticLoadObject<UFortMissionInfo>("/Game/Missions/Primary/EvacuateTheSurvivors/EvacuteTheSurvivors.EvacuteTheSurvivors");

		if (MissionInfo) {
			Log("AFortGameModeZone::FinishWorldInitialization: Loading mission " + MissionInfo->GetName().ToString());
			UFortMissionLibrary::LoadMission(World, MissionInfo);
			Log("AFortGameModeZone::FinishWorldInitialization: Loaded mission " + MissionInfo->GetName().ToString());
		}
	}

	ABuildingFoundation::SetupFoundations();
	UGameplayStatics::FlushLevelStreaming(UWorld::GetWorld());

	TArray<AActor*> ItemCollectors;
	UGameplayStatics::GetAllActorsOfClass(World, ABuildingItemCollectorActor::StaticClass(), &ItemCollectors);
	for (AActor* ItemCollectorActor : ItemCollectors) {
		ABuildingItemCollectorActor* ItemCollector = ItemCollectorActor->Cast<ABuildingItemCollectorActor>();
		if (ItemCollector) {
			if (!ItemCollector->Setup()) {
				//Log("AFortGameModeZone::FinishWorldInitialization: Failed to setup ItemCollector: " + ItemCollector->GetName().ToString());
			}
		}
	}

	TArray<AActor*> BGAConsumableSpawners;
	UGameplayStatics::GetAllActorsOfClass(World, ABGAConsumableSpawner::StaticClass(), &BGAConsumableSpawners);
	for (AActor* BGAConsumeableSpawnerActor : BGAConsumableSpawners) {
		ABGAConsumableSpawner* BGAConsumeableSpawner = BGAConsumeableSpawnerActor->Cast<ABGAConsumableSpawner>();
		if (BGAConsumeableSpawner) {
			if (!BGAConsumeableSpawner->AttemptSpawn()) {
				//Log("AFortGameModeZone::FinishWorldInitialization: Failed to spawn BGAConsumable: " + BGAConsumeableSpawner->GetName().ToString());
			}
		}
	}

	TArray<AActor*> BuildingContainers;
	UGameplayStatics::GetAllActorsOfClass(World, ABuildingContainer::StaticClass(), &BuildingContainers);
	for (AActor* BuildingContainerActor : BuildingContainers) {
		ABuildingContainer* BuildingContainer = BuildingContainerActor->Cast<ABuildingContainer>();
		if (BuildingContainer) {
			if (!BuildingContainer->Setup()) {
				//Log("AFortGameModeZone::FinishWorldInitialization: Failed to setup BuildingContainer: " + BuildingContainer->GetName().ToString());
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

void AFortGameModeZone::InitGameState(AFortGameModeZone* This) {
	InitGameStateOG(This);

	This->CreateAIDirector();
	This->CreateAIGoalManager();
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

	MH_CreateHook(
		(LPVOID)(GetOffsetFromVTable(AFortGameModeZone::GetDefaultObj(), Finder::FindAGameModeBase_InitGameStateVFT())),
		InitGameState,
		(LPVOID*)&InitGameStateOG
	);

	Log("Hooked AFortGameModeZone");
}
