#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

#include "FortniteGame/Public/AI/FortAIEncounterInfo.h"
class AFortAIPawn;
class AFortPlayerController;
class UFortAISpawnGroup;

struct FPendingSpawnInfo {
public:
	DefineUnrealStruct(FPendingSpawnInfo);

	DefineStructProperty(TSubclassOf<AFortAIPawn>, PawnClassToSpawn);
	DefineStructProperty(AActor*, SpawnPoint);
	DefineStructProperty(FVector, SpawnLocation);
	DefineStructProperty(FRotator, SpawnRotation);
	DefineStructProperty(AActor*, SpawnSource);
	DefineStructProperty(bool, bSpawnedFromExternalSpawner);
	DefineStructProperty(int32, SpawnSetIndex);
	DefineStructProperty(uint8, AIType);
	DefineStructProperty(AFortPlayerController*, TargetPlayer);
	DefineStructProperty(UFortAIEncounterInfo*, EncounterInfo);
	DefineStructProperty(float, DifficultyLevel);
	DefineStructProperty(UFortAISpawnGroup*, SpawnGroup);
	DefineStructProperty(FGuid, SpawnGroupGuid);
	DefineStructProperty(int32, EnemyIndexInSpawnGroup);
	DefineStructProperty(float, TimeToSpawn);
	DefineStructProperty(FGuid, PendingSpawnInfoGuid);
public:
	uint8 Padding[0x190];
};
