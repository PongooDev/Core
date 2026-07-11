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
class UFortWeaponItemDefinition;
class AFortAthenaPlaylistBase;
class UAthenaBattleBusItemDefinition;

class AFortGameModeAthena : public AFortGamePvPBase {
public:
	DefineUnrealClass(AFortGameModeAthena);

	DefineUProperty(int32, WarmupRequiredPlayerCount);
	DefineUProperty(int32, MaxPlayerCount);

	DefineUProperty(float, PlaylistHotfixOriginalGCFrequency);
	DefineUProperty(bool, bDisableGCOnServerDuringMatch);
	DefineUProperty(bool, bPlaylistHotfixChangedGCDisabling);

	DefineUProperty(TArray<AFortPlayerControllerAthena*>, AlivePlayers);
	DefineUProperty(int32, TeamAlivePlayers);
	DefineUProperty(bool, bAllowSpectateAfterDeath);
	DefineUProperty(bool, bAlwaysDBNO);
	DefineUProperty(AFortAthenaPlaylistBase*, FortAthenaPlaylist);
public:
	static inline bool (*ReadyToStartMatchOG)(AFortGameModeAthena* This);
	static bool ReadyToStartMatch(AFortGameModeAthena* This);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);

	static inline void (*FinishWorldInitializationOG)(AFortGameModeAthena* This, AFortWorldManager* WorldManager);
	static void FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager);

	void AddToAlivePlayers(AFortPlayerControllerAthena* PC);

	void RemoveFromAlivePlayers(AFortPlayerControllerAthena* PC, APlayerState* RemovalInstigator, APawn* FinisherPawn, UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause, bool bIsTeamSwitching);

	static inline int32 (*StartAircraftPhaseOG)(AFortGameModeAthena* This, bool bGoStraightToSafeZone);
	static int32 StartAircraftPhase(AFortGameModeAthena* This, bool bGoStraightToSafeZone);

	static inline uint8 (*PickTeamOG)(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);
	static uint8 PickTeam(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);

	static inline void (*InitGameStateOG)(AFortGameModeAthena* This);
	static void InitGameState(AFortGameModeAthena* This);

	static UAthenaBattleBusItemDefinition* GetBattleBusItemDefinition();

	static UClass* GetSupplyDropClass();

	static inline void (*PlacePlayerOnTeamOG)(AFortGameModeAthena* This, AFortPlayerController* FortPC);
	static void PlacePlayerOnTeam(AFortGameModeAthena* This, AFortPlayerController* FortPC);

	static inline void (*PreInitializeComponentsOG)(AFortGameModeAthena* This);
	static void PreInitializeComponents(AFortGameModeAthena* This);

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

		HookEveryVTableIdx(
			AFortGameModeAthena::StaticClass(),
			Finder::FindAGameModeBase_InitGameStateVFT(),
			InitGameState,
			(LPVOID*)&InitGameStateOG
		);

		HookEveryVTableIdx(
			AFortGameModeAthena::StaticClass(),
			Finder::FindAFortGameMode_PlacePlayerOnTeamVFT(),
			PlacePlayerOnTeam,
			(LPVOID*)&PlacePlayerOnTeamOG
		);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameModeAthena::GetDefaultObj(), Finder::FindAActor_PreInitializeComponentsVFT())),
			PreInitializeComponents,
			(LPVOID*)&PreInitializeComponentsOG
		);

		Log("Hooked AFortGameModeAthena");
	}
};