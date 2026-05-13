#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortWorld/FortWorldManager.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"

bool AFortGameModeAthena::ReadyToStartMatch(AFortGameModeAthena* This) {
	if (This->bWorldIsReady
		&& This->MatchState == MatchState::WaitingToStart
		&& This->NumPlayers >= This->WarmupRequiredPlayerCount) {

		Log(std::format("ReadyToStartMatch: Match ready to start! Players={}/{}, WorldReady={}",
			This->NumPlayers,
			This->WarmupRequiredPlayerCount,
			This->bWorldIsReady));
		return true;
	}

	return false;
}

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameMode::SpawnDefaultPawnForOG(This, NewPlayer, StartSpot);

	Log("SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

void AFortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	AFortGameStateAthena* GameState = This->GameState ? This->GameState->Cast<AFortGameStateAthena>() : nullptr;
	if (!GameState) {
		Log("FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return FinishWorldInitializationOG(This, WorldManager);
	}
	
	GameState->SetCurrentPlaylistId(This->CurrentPlaylistId);

	This->CreateAIDirector();
	This->CreateAIGoalManager();

	FinishWorldInitializationOG(This, WorldManager);
	This->bWorldIsReady = true;
}

void AFortGameModeAthena::BeginPlay(AFortGameModeAthena* This) {
	BeginPlayOG(This);

	if (Version::Fortnite_Version <= 1.72) {
		This->DefaultPawnClass = (UClass*)StaticLoadObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
		This->PlayerControllerClass = (UClass*)StaticLoadObject("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C");
		This->PlayerStateClass = AFortPlayerStateAthena::StaticClass();
	}
}

void AFortGameModeAthena::AddToAlivePlayers(AFortPlayerControllerAthena* PC) {
	void (*AddToAlivePlayersInternal)(AFortGameModeAthena* This, AFortPlayerControllerAthena* PC) = decltype(AddToAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_AddToAlivePlayers());
	AddToAlivePlayersInternal(this, PC);
}