#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Info.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"

class AGameSession;
class AGameStateBase;
class AHUD;
class APlayerState;
class ASpectatorPawn;
class UNetConnection;
class UPlayer;

class AGameModeBase : public AInfo {
public:
	DefineUnrealClass(AGameModeBase);

	DefineUProperty(AGameStateBase*, GameState);

	DefineUProperty(AGameSession*, GameSession);

	DefineUProperty(TSubclassOf<APawn>, DefaultPawnClass);
public:
	APawn* SpawnDefaultPawnFor(AController* NewPlayer, AActor* StartSpot);

	APawn* SpawnDefaultPawnAtTransform(AController* NewPlayer, const FTransform& SpawnTransform);

	void RestartPlayer(AController* NewPlayer);

	void FinishRestartPlayer(AController* NewPlayer, const FRotator& StartRotation);

	void HandleStartingNewPlayer(APlayerController* NewPlayer);
};