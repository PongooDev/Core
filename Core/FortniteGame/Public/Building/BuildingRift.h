#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingActor.h"
#include "FortniteGame/Public/Building/FortSpawnSlotData.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptDelegates.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AFortAIPawn;
class UEnvQuery;
class USphereComponent;
class UFortDamageSet;

class ERiftCosmeticState {
public:
	DefineUnrealEnum(ERiftCosmeticState);

	DefineEnumProperty(None);
	DefineEnumProperty(Intro);
	DefineEnumProperty(Idle);
	DefineEnumProperty(RampUp);
	DefineEnumProperty(ShouldDie);
};

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
