#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/AbilitySystemInterface.h"

#include "FortGameMode.h"
#include "FortniteGame/Public/FortItem/ItemAndCount.h"

class AFortPlayerController;
class AFortAIGoalManager;
class AFortAIDirector;
class ABuildingSMActor;
class AFortPlayerControllerZone;

class AFortGameModeZone : public AFortGameMode {
public:
	DefineUnrealClass(AFortGameModeZone);

	DefineUProperty(TArray<FItemAndCount>, StartingItems);

	DefineUProperty(AFortAIDirector*, AIDirector);
	DefineUProperty(AFortAIGoalManager*, AIGoalManager);
public:
	static inline void (*HandleStartingNewPlayerOG)(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer);
	static void HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer);
public:
	static void Hook();
};