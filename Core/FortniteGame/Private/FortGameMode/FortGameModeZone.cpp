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

void AFortGameModeZone::HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer) {
	HandleStartingNewPlayerOG(This, NewPlayer);

	Log("HandleStartingNewPlayer Called!");
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("HandleStartingNewPlayer: World is null!");
		return;
	}
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
}

APawn* AFortGameModeZone::SpawnDefaultPawnFor(AFortGameModeZone* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameMode::SpawnDefaultPawnFor(This, NewPlayer, StartSpot);

	auto PlayerState = (AFortPlayerStateZone*)NewPlayer->PlayerState;
	if (PlayerState) {
		if (PlayerState->AbilitySystemComponent) {
			UFortAbilitySet* FortAbilitySet = nullptr;

			if (Version::Fortnite_Version >= 2) {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
			}
			else {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");
			}

			TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface = TScriptInterface<IAbilitySystemInterface>();
			AbilitySystemInterface.ObjectPointer = PlayerState;
			AbilitySystemInterface.InterfacePointer = PlayerState->GetInterfaceAddress(IAbilitySystemInterface::StaticClass());

			UFortKismetLibrary::EquipFortAbilitySet(AbilitySystemInterface, FortAbilitySet, PlayerState);
		}
	}

	Log("AFortGameModeZone::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
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

	Log("Hooked AFortGameModeZone");
}