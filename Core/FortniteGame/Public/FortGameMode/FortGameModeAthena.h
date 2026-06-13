#pragma once
#include "pch.h"

#include "FortGamePvPBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

class AFortPlayerControllerAthena;
struct FVector;
class AFortAIDirector;
class UFortPlaylistAthena;
class AFortWorldManager;

class AFortGameModeAthena : public AFortGamePvPBase {
public:
	DefineUnrealClass(AFortGameModeAthena);

	DefineUProperty(int32, WarmupRequiredPlayerCount);

	DefineUProperty(float, PlaylistHotfixOriginalGCFrequency);
	DefineUProperty(bool, bDisableGCOnServerDuringMatch);
	DefineUProperty(bool, bPlaylistHotfixChangedGCDisabling);

	DefineUProperty(TArray<AFortPlayerControllerAthena*>, AlivePlayers);
public:
	static inline bool (*ReadyToStartMatchOG)(AFortGameModeAthena* This);
	static bool ReadyToStartMatch(AFortGameModeAthena* This);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);

	static inline void (*FinishWorldInitializationOG)(AFortGameModeAthena* This, AFortWorldManager* WorldManager);
	static void FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager);

	void AddToAlivePlayers(AFortPlayerControllerAthena* PC);

	static inline int32 (*StartAircraftPhaseOG)(AFortGameModeAthena* This, bool bGoStraightToSafeZone);
	static int32 StartAircraftPhase(AFortGameModeAthena* This, bool bGoStraightToSafeZone);

	static inline int32 (*PickTeamOG)(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);
	static int32 PickTeam(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);

	static void Hook() {
		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_ReadyToStartMatch()), ReadyToStartMatch, (LPVOID*)&ReadyToStartMatchOG);
		HookEveryVTable(
			AFortGameModeAthena::StaticClass(),
			AFortGameModeAthena::StaticClass()->GetFunction("Function /Script/Engine.GameMode.ReadyToStartMatch"),
			ReadyToStartMatch,
			(LPVOID*)&ReadyToStartMatchOG
		);

		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_SpawnDefaultPawnFor()), SpawnDefaultPawnFor, (LPVOID*)&SpawnDefaultPawnForOG);
		HookEveryVTable(
			AFortGameModeAthena::StaticClass(),
			AFortGameModeAthena::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"),
			SpawnDefaultPawnFor,
			(LPVOID*)&SpawnDefaultPawnForOG
		);

		//MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_FinishWorldInitialization()), FinishWorldInitialization, (LPVOID*)&FinishWorldInitializationOG);
		HookEveryVTableIdx(
			AFortGameModeAthena::StaticClass(),
			Finder::FindAFortGameMode_FinishWorldInitializationVFT(),
			FinishWorldInitialization,
			(LPVOID*)&FinishWorldInitializationOG
		);

		if (Finder::FindAFortGameModeAthena_StartAircraftPhase()) {
			MH_CreateHook(
				(LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_StartAircraftPhase()),
				StartAircraftPhase,
				(LPVOID*)&StartAircraftPhaseOG
			);
		}

		HookEveryVTableIdx(
			AFortGameModeAthena::StaticClass(),
			Finder::FindAFortGameMode_PickTeamVFT(),
			PickTeam,
			(LPVOID*)&PickTeamOG
		);

		Log("Hooked AFortGameModeAthena");
	}
};