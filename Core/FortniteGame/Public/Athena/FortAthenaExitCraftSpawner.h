#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"


class UFortAthenaExitCraftInfo;

class AFortAthenaExitCraft;
class AFortAthenaExitCraftSpawner;

class AFortAthenaExitCraftSpawner : public AActor {
public:
	DefineUnrealClass(AFortAthenaExitCraftSpawner);

	DefineUProperty(UFortAthenaExitCraftInfo*, ExitCraftInfo);
	DefineUProperty(FName, SpawnerSpecialActorID);
public:
	void StartExitCraftSpawnTimer() {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("StartExitCraftSpawnTimer");

		if (Func)
			Call(Func);
	}

	void DestroyBlockingActors() {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("DestroyBlockingActors");

		if (Func)
			Call(Func);
	}

	static void SpawnExitCraft(AFortAthenaExitCraftSpawner* This);

	static inline void (*BeginPlayOG)(AFortAthenaExitCraftSpawner* This);
	static void BeginPlay(AFortAthenaExitCraftSpawner* This);

	static void Hook();
};
