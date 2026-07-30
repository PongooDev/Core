#include "pch.h"
#include "FortniteGame/Public/FortGameModeOutpost.h"

#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"

void AFortGameModeOutpost::FinishWorldInitialization(AFortGameModeOutpost* This, AFortWorldManager* WorldManager) {
	This->CreateAIDirector();
	This->CreateAIGoalManager();

	FinishWorldInitializationOG(This, WorldManager);
	This->bWorldIsReady = true;
}

APawn* AFortGameModeOutpost::SpawnDefaultPawnFor(AFortGameModeOutpost* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameModeZone::SpawnDefaultPawnFor(This, NewPlayer, StartSpot);

	Log("AFortGameModeOutpost::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

bool AFortGameModeOutpost::ReadyToStartMatch(AFortGameModeOutpost* This) {
	return ReadyToStartMatchOG(This);
}