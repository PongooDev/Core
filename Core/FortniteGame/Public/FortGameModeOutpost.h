#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortGameModeZone.h"

class AFortGameModeOutpost : public AFortGameModeZone {
public:
	DefineUnrealClass(AFortGameModeOutpost);
public:
	static inline void (*FinishWorldInitializationOG)(AFortGameModeOutpost* This, AFortWorldManager* WorldManager);
	static void FinishWorldInitialization(AFortGameModeOutpost* This, AFortWorldManager* WorldManager);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeOutpost* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameModeOutpost* This, AController* NewPlayer, AActor* StartSpot);

	static inline bool (*ReadyToStartMatchOG)(AFortGameModeOutpost* This);
	static bool ReadyToStartMatch(AFortGameModeOutpost* This);

	static void Hook() {
		HookVTableIdx(
			AFortGameModeOutpost::GetDefaultObj(),
			Finder::FindAFortGameMode_FinishWorldInitializationVFT(),
			FinishWorldInitialization,
			(LPVOID*)&FinishWorldInitializationOG
		);

		HookVTable(
			AFortGameModeOutpost::GetDefaultObj(),
			AFortGameModeOutpost::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"),
			SpawnDefaultPawnFor,
			(LPVOID*)&SpawnDefaultPawnForOG
		);

		HookEveryVTable(
			AFortGameModeOutpost::StaticClass(),
			AFortGameModeOutpost::StaticClass()->GetFunction("Function /Script/Engine.GameMode.ReadyToStartMatch"),
			ReadyToStartMatch,
			(LPVOID*)&ReadyToStartMatchOG
		);

		Log("Hooked AFortGameModeOutpost");
	}
};