#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingActor.h"
#include "FortniteGame/Public/Building/FortSpawnSlotData.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"

class AFortAIPawn;
class UEnvQuery;
class USphereComponent;
class UFortDamageSet;

class ABuildingRift : public ABuildingActor {
public:
	DefineUnrealClass(ABuildingRift);

	DefineUProperty(UFortDamageSet*, DamageSet);
	DefineUProperty(float, DistToTarget);
	DefineUProperty(bool, bSendMissionEvents);
	DefineUProperty(bool, bDelayDeath);
	DefineUProperty(float, MinSpawnDelay);
	DefineUProperty(float, MaxSpawnDelay);
	DefineUProperty(float, CosmeticStateIdleDelay);
	DefineUProperty(float, IntroToBeginSpawningDelay);
	DefineUProperty(UEnvQuery*, EnvironmentQuery);
	DefineUProperty(float, QueryRadius);
	DefineUProperty(TArray<FVector>, FreeSpawnSlots);
	DefineUProperty(TArray<FFortSpawnSlotData>, UsedSpawnSlots);
	DefineUProperty(bool, bRiftIsVisible);
	DefineUProperty(bool, bRiftIsActive);
	DefineUProperty(bool, bIsReadyToSpawnAI);
	DefineUProperty(bool, bHasBeenInitialized);
	DefineUProperty(bool, bHasBadRiftSlots);
	DefineUProperty(bool, bCreateOverlapSphere);
	DefineUProperty(float, OverlapSphereRadius);
	DefineUProperty(USphereComponent*, OverlapSphereComponent);
	DefineUProperty(FMulticastScriptDelegate, OnBuildingRiftSpawnedAI);
	DefineUProperty(FName, DestructionLootTierGroup);
	DefineUProperty(FRotator, LootDropTossDirection);
	DefineUProperty(float, LootDropConeHalfAngle);
	DefineUProperty(float, LootDropSpeed);
	DefineUProperty(uint8, CosmeticState);
public:
	void BroadcastRiftSpawningAI();

	void OnRep_CosmeticState();

	void OnRep_RiftIsActive();

	void OnRep_RiftIsVisible();

	static void Hook();
};
