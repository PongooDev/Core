#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortGamePvPBase.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameSession.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "FortniteGame/Public/Athena/FortSpawnActorData.h"
#include "FortniteGame/Public/Athena/FortSpawnActorInfo.h"
#include "FortniteGame/Public/FortEnums.h"

class AFortPlayerControllerAthena;
struct FVector;
class AFortAIDirector;
class UFortPlaylistAthena;
class AFortWorldManager;
class UFortWeaponItemDefinition;
class AFortAthenaPlaylistBase;
class UAthenaBattleBusItemDefinition;
class UAthenaAISettings;

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
	DefineUProperty(UAthenaAISettings*, AISettings);
	DefineUProperty(TArray<FVector>, SafeZoneLocations);
	DefineUProperty(TArray<FFortSpawnActorData>, SpawnActorDataList);
	DefineUProperty(int32, SafeZonePhase);
public:
	void OverrideAISettings(UAthenaAISettings* NewAISettings);

	static inline bool (*ReadyToStartMatchOG)(AFortGameModeAthena* This);
	static bool ReadyToStartMatch(AFortGameModeAthena* This);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot);

	bool SetupPlaylist();

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

	static void SendEndOfMatchTo(AFortPlayerControllerAthena* PC, bool bMatchEnded = false);

	static inline bool (*StartEndGamePhaseOG)(AFortGameModeAthena* This, AFortPlayerControllerAthena* WinningPlayer, APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);
	static bool StartEndGamePhase(AFortGameModeAthena* This, AFortPlayerControllerAthena* WinningPlayer, APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);

	static inline bool (*StartEndGamePhaseTeamOG)(AFortGameModeAthena* This, int32 TeamIndex, APlayerState* PlayerState, int32 Place, APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);
	static bool StartEndGamePhaseTeam(AFortGameModeAthena* This, int32 TeamIndex, APlayerState* PlayerState, int32 Place, APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause);

	void UpdateSpawnActorListDuringSafeZone(EAthenaGamePhaseStep GamePhaseStep);

	void SpawnFortSpawnActors(AFortGameModeAthena* This, FFortSpawnActorData* SpawnActorData);

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

		if (Finder::FindAFortGameModeAthena_StartEndGamePhase()) {
			MH_CreateHook(
				(LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_StartEndGamePhase()),
				StartEndGamePhase,
				(LPVOID*)&StartEndGamePhaseOG
			);
		}

		if (Finder::FindAFortGameModeAthena_StartEndGamePhaseTeam()) {
			MH_CreateHook(
				(LPVOID)(ImageBase + Finder::FindAFortGameModeAthena_StartEndGamePhaseTeam()),
				StartEndGamePhaseTeam,
				(LPVOID*)&StartEndGamePhaseTeamOG
			);
		}

		Log("Hooked AFortGameModeAthena");
	}
};
