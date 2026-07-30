#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"

#include "FortniteGame/Public/FortGameMode.h"
#include "FortniteGame/Public/Items/ItemAndCount.h"
#include "FortniteGame/Public/FortEnums.h"

class AFortPlayerController;
class AFortAIGoalManager;
class AFortAIDirector;
class ABuildingSMActor;
class AFortPlayerControllerZone;
class AFortWorldManager;

class AFortGameModeZone : public AFortGameMode {
public:
	DefineUnrealClass(AFortGameModeZone);

	DefineUProperty(TArray<FItemAndCount>, StartingItems);
	DefineUProperty(AFortAIDirector*, AIDirector);
	DefineUProperty(AFortAIGoalManager*, AIGoalManager);
	DefineUProperty(ESubGame, AssociatedSubGame);
public:
	static inline void (*HandleStartingNewPlayerOG)(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer);
	static void HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer);

	void CreateAIDirector();

	void CreateAIGoalManager();

	static inline void (*FinishWorldInitializationOG)(AFortGameModeZone* This, AFortWorldManager* WorldManager);
	static void FinishWorldInitialization(AFortGameModeZone* This, AFortWorldManager* WorldManager);

	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameModeZone* This, AController* NewPlayer, AActor* StartSpot);
	static APawn* SpawnDefaultPawnFor(AFortGameModeZone* This, AController* NewPlayer, AActor* StartSpot);

	static void AddInactivePlayerHK(AFortGameModeZone* This, APlayerState* PlayerState, APlayerController* PC);

	static inline void (*InitGameStateOG)(AFortGameModeZone* This);
	static void InitGameState(AFortGameModeZone* This);

	static void Hook();
};