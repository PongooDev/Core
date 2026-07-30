#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

class AFortPlayerController;
class AFortWorldManager;
class AFortMissionManager;
class AFortGameSession;
class UFortPlaylistManager;

class AFortGameMode : public AGameMode {
public:
	DefineUnrealClass(AFortGameMode);

	DefineBitfieldUProperty(bWorldIsReady);

	DefineUProperty(int32, CurrentPlaylistId);
	DefineUProperty(FName, CurrentPlaylistName);
	DefineUProperty(TSubclassOf<AFortMissionManager>, MissionManagerClass);
	DefineBitfieldUProperty(bTeamGame);
	DefineUProperty(bool, bDBNOEnabled);
	DefineUProperty(AFortGameSession*, FortGameSession);
	DefineUProperty(UFortPlaylistManager*, PlaylistManager);
public:
	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameMode* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameMode* This, AController* NewPlayer, AActor* StartSpot);

	bool SpawnNativePlayerBot(AActor* SpawnPoint = nullptr);

	bool SpawnPlayerBot(AActor* SpawnPoint = nullptr);

	uint8 PickTeam(uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);

	static inline UClass** (*GetGameSessionClassOG)(AFortGameMode* This, UClass** result);
	static UClass** GetGameSessionClass(AFortGameMode* This, UClass** result);

	static inline void (*RestartPlayerOG)(AFortGameMode* This, AController* NewPlayer);
	static void RestartPlayerHK(AFortGameMode* This, AController* NewPlayer);

	static inline void (*FinishWorldInitializationOG)(AFortGameMode* This, AFortWorldManager* WorldManager);
	static void FinishWorldInitialization(AFortGameMode* This, AFortWorldManager* WorldManager);

	static void AddInactivePlayerHK(AFortGameMode* This, APlayerState* PlayerState, APlayerController* PC);

	static inline bool (*ReadyToStartMatchOG)(AFortGameMode* This);
	static bool ReadyToStartMatch(AFortGameMode* This);

	static inline void (*PreInitializeComponentsOG)(AFortGameMode* This);
	static void PreInitializeComponents(AFortGameMode* This);

	void InitializeTeams();

	void SetCurrentPlaylistName(FName NewPlaylistName);

	void SetCurrentPlaylistId(int32 NewPlaylistID);

	void PlacePlayerOnTeamVFT(AFortPlayerController* FortPC);

	static void Hook() {
		//CreateVTableOriginal(AFortGameMode::GetDefaultObj(), AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"), (LPVOID*)&SpawnDefaultPawnForOG);
		HookVTable(
			AFortGameMode::GetDefaultObj(),
			AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"),
			SpawnDefaultPawnFor,
			(LPVOID*)&SpawnDefaultPawnForOG
		);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameMode::GetDefaultObj(), Finder::FindAGameModeBase_GetGameSessionClassVFT())),
			GetGameSessionClass,
			(LPVOID*)&GetGameSessionClassOG
		);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameMode::GetDefaultObj(), AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.RestartPlayer"))),
			RestartPlayerHK,
			(LPVOID*)&RestartPlayerOG
		);

		HookVTableIdx(
			AFortGameMode::GetDefaultObj(),
			Finder::FindAFortGameMode_FinishWorldInitializationVFT(),
			FinishWorldInitialization,
			(LPVOID*)&FinishWorldInitializationOG
		);

		HookVTableIdx(
			AFortGameMode::GetDefaultObj(),
			Finder::FindAGameMode_AddInactivePlayerVFT(),
			AddInactivePlayerHK,
			nullptr
		);

		HookVTable(
			AFortGameMode::StaticClass(),
			AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameMode.ReadyToStartMatch"),
			ReadyToStartMatch,
			(LPVOID*)&ReadyToStartMatchOG
		);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameMode::GetDefaultObj(), Finder::FindAActor_PreInitializeComponentsVFT())),
			PreInitializeComponents,
			(LPVOID*)&PreInitializeComponentsOG
		);

		Log("AFortGameMode Hooked!");
	}
};