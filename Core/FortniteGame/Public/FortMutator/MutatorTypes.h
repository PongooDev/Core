#pragma once
#include "pch.h"

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
#include "FortniteGame/Public/FortItem/ItemAndCount.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "MutatorObjectData.h"

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

struct FAshtonStoneState {
public:
	DefineUnrealStruct(FAshtonStoneState);

	DefineStructProperty(EAshtonStoneType, StoneType);
	DefineStructProperty(EAshtonStoneStateType, StoneState);
	DefineStructProperty(FGameplayTag, GameplayTag);
	DefineStructProperty(float, SpawnTime);
	DefineStructProperty(bool, bHasEverSpawned);
	DefineStructProperty(int32, SpawnDataIdx);
public:
	uint8 Padding[0x18];
};

struct FFortAISpawnGroupAssetData {
public:
	DefineUnrealStruct(FFortAISpawnGroupAssetData);

	DefineStructProperty(UFortAISpawnGroup*, SpawnGroup);
	DefineStructProperty(FScalableFloat, Weight);
	DefineStructProperty(FScalableFloat, MinNumber);
	DefineStructProperty(FScalableFloat, MaxNumber);
public:
	uint8 Padding[0x68];
};

struct FFortAIUpgradeEffectAssetData {
public:
	DefineUnrealStruct(FFortAIUpgradeEffectAssetData);

	DefineStructProperty(FScalableFloat, ApplyingEffectPercentage);
	DefineStructProperty(TSubclassOf<UGameplayEffect>, GameplayEffect);
public:
	uint8 Padding[0x28];
};

struct FFortAIAssetData {
public:
	DefineUnrealStruct(FFortAIAssetData);

	DefineStructProperty(float, MovingZoneSpawningRate);
	DefineStructProperty(float, MovingZoneSpawningRadiusMultiplier);
	DefineStructProperty(float, SpawnSafeZoneRadiusRatio);
	DefineStructProperty(float, SpawnMovingSafeZoneRadiusRatio);
	DefineStructProperty(float, SpawnTraceStartHeightOffset);
	DefineStructProperty(float, SpawnTraceLength);
	DefineStructProperty(FScalableFloat, KillScore);
	DefineStructProperty(FScalableFloat, SpawnCount);
	DefineStructProperty(TArray<FFortAISpawnGroupAssetData>, SpawnGroupAssets);
	DefineStructProperty(TArray<FFortAIUpgradeEffectAssetData>, BaseUpgradeEffectAssets);
	DefineStructProperty(TArray<FFortAIUpgradeEffectAssetData>, ExtraUpgradeEffectAssets);
public:
	uint8 Padding[0x88];
};

struct FFortAthenaMutator_SurvivalObjectiveData {
public:
	DefineUnrealStruct(FFortAthenaMutator_SurvivalObjectiveData);

	DefineStructProperty(TSubclassOf<ABuildingActor>, BuildingActorObjectiveClass);
	DefineStructProperty(FScalableFloat, SpawnDistanceFromGround);
	DefineStructProperty(int32, ActivationSafezoneIndex);
	DefineStructProperty(bool, bClearAreaOnSpawn);
	DefineStructProperty(float, ClearAreaRadiusOverride);
	DefineStructProperty(float, ClearAreaHalfHeightOverride);
	DefineStructProperty(FGameplayTagQuery, POIFilterQuery);
	DefineStructProperty(int32, RandomizedPOICount);
	DefineStructProperty(TArray<int32>, ExtraSafezoneIndexes);
	DefineStructProperty(ABuildingActor*, SpawnedBuildingActorObjective);
	DefineStructProperty(FGameplayTag, SpecialActorTag);
	DefineStructProperty(FSlateBrush, SpecialActorMinimapIconBrush);
	DefineStructProperty(FVector2D, SpecialActorMinimapIconScale);
	DefineStructProperty(FSlateBrush, SpecialActorCompassIconBrush);
	DefineStructProperty(FVector2D, SpecialActorCompassIconScale);
	DefineStructProperty(FName, SpecialActorID);
	DefineBitfieldStructProperty(bEndMatchOnDestroy);
	DefineBitfieldStructProperty(bSpawnOnPOI);
	DefineBitfieldStructProperty(bIsSpecialActor);
public:
	uint8 Padding[0x1D8];
};

struct FFortDelayRTMMData {
public:
	DefineUnrealStruct(FFortDelayRTMMData);

	DefineStructProperty(bool, bDelayRTTM);
	DefineStructProperty(float, MinRTTMDelay);
	DefineStructProperty(float, MaxRTTMDelay);
public:
	uint8 Padding[0xC];
};

struct FFortItemDeliverySupplyDropMutatorData {
public:
	DefineUnrealStruct(FFortItemDeliverySupplyDropMutatorData);

	DefineStructProperty(bool, bShouldApplyMutator);
	DefineStructProperty(FScalableFloat, NumDeliveryItemsToSpawn);
	DefineStructProperty(UEnvQuery*, SupplyDropPlacementQuery);
public:
	uint8 Padding[0x30];
};

struct FFortObjectiveAreaInstanceData {
public:
	DefineUnrealStruct(FFortObjectiveAreaInstanceData);

	DefineStructProperty(FVector, Location);
	DefineStructProperty(ABuildingGameplayActorSpawnMachine*, SpawnMachine);
	DefineStructProperty(AFortAthenaSupplyDrop*, MainObjective);
	DefineStructProperty(TArray<ABuildingActor*>, SubObjectives);
public:
	uint8 Padding[0x30];
};

struct FFortObjectiveMessagesAssetData {
public:
	DefineUnrealStruct(FFortObjectiveMessagesAssetData);

	DefineStructProperty(FAthenaGameMessageData, GameMsg_SafeZonePreObjective);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_Objectives);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_MainObjectiveSpawned);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_SafeZonePostObjective);
	DefineStructProperty(FAthenaGameMessageData, GameMsg_MoveToNextArea);
public:
	uint8 Padding[0x118];
};

struct FFortSubObjectiveSpawnData {
public:
	DefineUnrealStruct(FFortSubObjectiveSpawnData);

	DefineStructProperty(FScalableFloat, ScoreMultiplier);
	DefineStructProperty(FScalableFloat, SpawnCount);
	DefineStructProperty(TSubclassOf<ABuildingActor>, ObjectClass);
public:
	uint8 Padding[0xC8];
};

struct FFortSupplyDropMutatorData {
public:
	DefineUnrealStruct(FFortSupplyDropMutatorData);

	DefineStructProperty(FName, SupplyDropID);
	DefineStructProperty(bool, bShouldCenterGroundCheckAtFoundLocation);
	DefineStructProperty(TArray<FFortItemDeliverySupplyDropMutatorData>, ItemDeliveryMutatorPerSafeZonePhase);
public:
	uint8 Padding[0x20];
};

struct FGravityMovementData {
public:
	DefineUnrealStruct(FGravityMovementData);

	DefineStructProperty(float, GravityZScale);
	DefineStructProperty(float, VehicleGravityZScale);
	DefineStructProperty(float, JumpZVelocityOverride);
	DefineStructProperty(float, JumpHorizontalAccelerationOverride);
	DefineStructProperty(float, JumpHorizontalVelocityOverride);
public:
	uint8 Padding[0x14];
};

struct FItemLoadoutContainer {
public:
	DefineUnrealStruct(FItemLoadoutContainer);

	DefineStructProperty(TArray<FItemAndCount>, Loadout);
public:
	uint8 Padding[0x10];
};

struct FItemLoadoutTeamMap {
public:
	DefineUnrealStruct(FItemLoadoutTeamMap);

	DefineStructProperty(uint8, TeamIndex);
	DefineStructProperty(uint8, LoadoutIndex);
	DefineStructProperty(EAthenaInventorySpawnOverride, UpdateOverrideType);
	DefineStructProperty(EAthenaLootDropOverride, DropAllItemsOverride);
public:
	uint8 Padding[0x4];
};

struct FItemsToDropOnDeath {
public:
	DefineUnrealStruct(FItemsToDropOnDeath);

	DefineStructProperty(UFortWorldItemDefinition*, ItemToDrop);
	DefineStructProperty(FScalableFloat, NumberToDrop);
public:
	uint8 Padding[0x28];
};

struct FItemsToGive {
public:
	DefineUnrealStruct(FItemsToGive);

	DefineStructProperty(UFortWorldItemDefinition*, ItemToDrop);
	DefineStructProperty(FScalableFloat, NumberToGive);
public:
	uint8 Padding[0x28];
};

struct FItemsToGiveAtPhase {
public:
	DefineUnrealStruct(FItemsToGiveAtPhase);

	DefineStructProperty(UFortWorldItemDefinition*, ItemToDrop);
	DefineStructProperty(FScalableFloat, NumberToGive);
public:
	uint8 Padding[0x28];
};

struct FMapLocation {
public:
	DefineUnrealStruct(FMapLocation);

	DefineStructProperty(FText, Text);
	DefineStructProperty(FVector2D, Position);
	DefineStructProperty(FLinearColor, Color);
	DefineStructProperty(FGameplayTag, LocationTag);
public:
	uint8 Padding[0x88];
};

struct FMashDifficultySettings {
public:
	DefineUnrealStruct(FMashDifficultySettings);

	DefineStructProperty(FScalableFloat, SpawnCountPlayerCountMultiplier);
	DefineStructProperty(FScalableFloat, SpawnCountDifficultyMultiplier);
	DefineStructProperty(FScalableFloat, AIEffectPlayerCountMultiplier);
	DefineStructProperty(FScalableFloat, AIEffectDifficultyMultiplier);
	DefineStructProperty(float, SpawnCountMultiplier);
	DefineStructProperty(float, AIEffectMultiplier);
public:
	uint8 Padding[0x88];
};

struct FMashPlayerScoreInstanceData {
public:
	DefineUnrealStruct(FMashPlayerScoreInstanceData);

	DefineStructProperty(FString, PlayerName);
	DefineStructProperty(int32, PlayerID);
	DefineStructProperty(int32, Score);
public:
	uint8 Padding[0x18];
};

struct FObjectiveSpecialActorContainer {
public:
	DefineUnrealStruct(FObjectiveSpecialActorContainer);

	DefineStructProperty(AActor*, TheSpawnedObjective);
public:
	uint8 Padding[0x10];
};

struct FPartSwapData {
public:
	DefineUnrealStruct(FPartSwapData);

	DefineStructProperty(TArray<UCustomCharacterPart*>, PartsToSwapIn);
public:
	uint8 Padding[0x10];
};

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

struct FSkyCapPositionData {
public:
	DefineUnrealStruct(FSkyCapPositionData);

	DefineStructProperty(FScalableFloat, MoveTime);
	DefineStructProperty(FScalableFloat, Height);
	DefineStructProperty(FScalableFloat, WaitTime);
public:
	uint8 Padding[0x60];
};

struct FTeamSettingsData {
public:
	DefineUnrealStruct(FTeamSettingsData);

	DefineStructProperty(UFortTeamSettingsComponent*, TeamSettings);
public:
	uint8 Padding[0x18];
};

struct FTimeOfDayPhase {
public:
	DefineUnrealStruct(FTimeOfDayPhase);

	DefineStructProperty(float, Time);
	DefineStructProperty(FText, DisplayName);
public:
	uint8 Padding[0x20];
};

struct FTimeOfDaySpeed {
public:
	DefineUnrealStruct(FTimeOfDaySpeed);

	DefineStructProperty(float, Speed);
	DefineStructProperty(FText, DisplayName);
public:
	uint8 Padding[0x20];
};
