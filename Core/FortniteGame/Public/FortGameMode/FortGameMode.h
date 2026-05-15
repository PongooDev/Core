#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameMode.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"

class AFortPlayerController;

class AFortGameMode : public AGameMode {
public:
	DefineUnrealClass(AFortGameMode);

	DefineBitfieldUProperty(bWorldIsReady);

	DefineUProperty(int32, CurrentPlaylistId);
	DefineUProperty(FName, CurrentPlaylistName);
public:
	static inline APawn* (*SpawnDefaultPawnForOG)(AFortGameMode* This, AController* NewPlayer, AActor* StartSpot);

	bool SpawnPlayerBot(AActor* SpawnPoint = nullptr);

	uint8 PickTeam(uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor);

	static inline UClass** (*GetGameSessionClassOG)(AFortGameMode* This, UClass** result);
	static UClass** GetGameSessionClass(AFortGameMode* This, UClass** result);

	static void Hook() {
		CreateVTableOriginal(AFortGameMode::GetDefaultObj(), AFortGameMode::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.SpawnDefaultPawnFor"), (LPVOID*)&SpawnDefaultPawnForOG);

		MH_CreateHook(
			(LPVOID)(GetOffsetFromVTable(AFortGameMode::GetDefaultObj(), Finder::FindAGameModeBase_GetGameSessionClassVFT())),
			GetGameSessionClass,
			(LPVOID*)&GetGameSessionClassOG
		);

		Log("AFortGameMode Hooked!");
	}
};