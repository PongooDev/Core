#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "FortniteGame/Public/FortEnums.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "FortniteGame/Public/Items/ItemAndCount.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "FortniteGame/Public/Athena/Modifiers/MutatorObjectData.h"

class AActor;
class AAthenaBarrierFlag;
class AAthenaBigBaseWall;
class AAthenaFillFloor;
class AAthenaGoosePickup;
class AAthenaSkyCap;
class ABuildingActor;
class ABuildingGameplayActorSpawnMachine;
class AFortAthenaSupplyDrop;
class AFortAthenaVehicle;
class AFortGameModeAthena;
class AFortGameStateAthena;
class AFortPlayerController;
class AFortPlayerControllerAthena;
class AFortPlayerPawnAthena;
class AFortPlayerStateAthena;
class UAnimInstance;
class UCustomCharacterPart;
class UDataAsset;
class UEnvQuery;
class UFortAIEncounterInfo;
class UFortAISpawnGroup;
class UFortAthenaExitCraftInfo;
class UFortAthenaMutator_SpawningPolicyData;
class UFortDifficultyEncounterSettings;
class UFortGadgetItemDefinition;
class UFortMissionInfo;
class UFortMovementComp_AIChar;
class UFortServerBotManagerAthena;
class UFortTeamSettingsComponent;
class UFortWorldItemDefinition;
class UGameplayEffect;
class UMaterialInstance;
class USkeletalMesh;
class USoundCue;
class UStaticMesh;
class UTexture2D;

struct FPlaneLootEntry {
public:
	DefineUnrealStruct(FPlaneLootEntry);

	DefineStructProperty(FName, LootTableName);
	DefineStructProperty(FScalableFloat, LootWeight);
public:
	uint8 Padding[0x28];
};

struct FPlaneLootHeightEntry {
public:
	DefineUnrealStruct(FPlaneLootHeightEntry);

	DefineStructProperty(FScalableFloat, RelativeWeight);
	DefineStructProperty(FScalableFloat, MinHeightOffGround);
	DefineStructProperty(FScalableFloat, MaxHeightOffGround);
	DefineStructProperty(TArray<FPlaneLootEntry>, LootEntries);
public:
	uint8 Padding[0x70];
};

struct FPlaneLootTypeEntry {
public:
	DefineUnrealStruct(FPlaneLootTypeEntry);

	DefineStructProperty(FScalableFloat, RelativeWeight);
	DefineStructProperty(TArray<FPlaneLootHeightEntry>, LootHeightEntries);
public:
	uint8 Padding[0x30];
};

struct FPlaneSquadInfo {
public:
	DefineUnrealStruct(FPlaneSquadInfo);

	DefineStructProperty(TArray<AFortPlayerControllerAthena*>, Controllers);
	DefineStructProperty(AFortPlayerStateAthena*, LastDriver);
	DefineStructProperty(uint8, SquadId);
	DefineStructProperty(int32, PlanesRemaining);
	DefineStructProperty(FName, LastCollectedChest);
	DefineStructProperty(AFortAthenaVehicle*, LastCollectedChestVehicle);
	DefineStructProperty(float, LastCollectedChestServerTime);
public:
	uint8 Padding[0x48];
};

struct FPlaneSquadInfoArray {
public:
	DefineUnrealStruct(FPlaneSquadInfoArray);

	DefineStructProperty(TArray<FPlaneSquadInfo>, SquadData);
	DefineStructProperty(TArray<AFortPlayerControllerAthena*>, RemovedControllers);
public:
	uint8 Padding[0x130];
};

class AFortAthenaMutator_Goose : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Goose);

	DefineUProperty(TArray<TSubclassOf<UGameplayEffect>>, PlaneStatusEffects);
	DefineUProperty(FScalableFloat, AircraftPhaseLength);
	DefineUProperty(FVector, PlaneSpawnLocation);
	DefineUProperty(FScalableFloat, InitialRadius);
	DefineUProperty(FScalableFloat, PlaneSpawnHeight);
	DefineUProperty(FScalableFloat, PlaneSkyCapOffset);
	DefineUProperty(FScalableFloat, PlaneSpawnPitch);
	DefineUProperty(FScalableFloat, InitialNumOfSleepingPlanes);
	DefineUProperty(FScalableFloat, PlaneSpawnStormRadiusMultiplier);
	DefineUProperty(FScalableFloat, GroundTestValue);
	DefineUProperty(FScalableFloat, DriverRespawnScalar);
	DefineUProperty(TSubclassOf<AFortAthenaVehicle>, VehicleClass);
	DefineUProperty(TSubclassOf<UGameplayEffect>, SiphonEffectForPlanes);
	DefineUProperty(FScalableFloat, NumberOfPlanesPerSquad);
	DefineUProperty(TArray<FPlaneLootTypeEntry>, LootTypeEntries);
	DefineUProperty(FScalableFloat, HowOftenToUpdatePickupCount);
	DefineUProperty(FName, DefaultLootTableName);
	DefineUProperty(FScalableFloat, DefaultDeployAltitude);
	DefineUProperty(FScalableFloat, DefaultPickupSpawnHeight);
	DefineUProperty(TSubclassOf<AAthenaGoosePickup>, PickupClass);
	DefineUProperty(FScalableFloat, MaxNumOfAlivePickups);
	DefineUProperty(TSubclassOf<AActor>, BoundsXYSplineClass);
	DefineUProperty(FPlaneSquadInfoArray, SquadInfoArray);
	DefineUProperty(TArray<AFortAthenaVehicle*>, SpawnedVehicles);
	DefineUProperty(TArray<AAthenaGoosePickup*>, SpawnedPickups);
};
