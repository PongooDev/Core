#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "FortniteGame/Public/Items/ItemsToSpawn.h"
#include "FortniteGame/Public/Capture/AthenaBigBaseWall.h"
#include "Engine/Source/Runtime/Core/Public/Math/TransformNonVectorized.h"
#include "FortniteGame/Public/Capture/AthenaBarrierFlag.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "FortniteGame/Public/Capture/AthenaBarrierObjective.h"
#include "FortniteGame/Public/FortGameStateAthena.h"


class AAthenaBarrierFlag;

class AAthenaBarrierObjective;
class UCustomCharacterPart;
class UMaterialInterface;
class UStaticMesh;

struct FBarrierHeadData {
public:
	DefineUnrealStruct(FBarrierHeadData);

	DefineStructProperty(TArray<UCustomCharacterPart*>, PartsToSwapIn);
public:
	uint8 Padding[0x10];
};

struct FBarrierMountedTurretData {
public:
	DefineUnrealStruct(FBarrierMountedTurretData);

	DefineStructProperty(TArray<UMaterialInterface*>, MaterialOverrides);
	DefineStructProperty(UMaterialInterface*, BaseMaterialOverride);
public:
	uint8 Padding[0x18];
};

struct FBarrierTeamState {
public:
	DefineUnrealStruct(FBarrierTeamState);

	DefineStructProperty(uint8, TeamNum);
	DefineStructProperty(EBarrierFoodTeam, FoodTeam);
	DefineStructProperty(AAthenaBarrierFlag*, ObjectiveFlag);
	DefineStructProperty(AAthenaBarrierObjective*, ObjectiveObject);
	DefineStructProperty(bool, bRespawnEnabled);
public:
	uint8 Padding[0x28];
};

class AFortAthenaMutator_Barrier : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Barrier);

	DefineUProperty(TSubclassOf<AAthenaBigBaseWall>, BigBaseWallClass);
	DefineUProperty(TSubclassOf<AAthenaBarrierFlag>, ObjectiveFlag);
	DefineUProperty(FBarrierHeadData, FoodHeadData);
	DefineUProperty(FBarrierMountedTurretData, MountedTurretOverrideMaterials);
	DefineUProperty(AAthenaBigBaseWall*, BigBaseWall);
	DefineUProperty(FBarrierTeamState, Team_0_State);
	DefineUProperty(FBarrierTeamState, Team_1_State);
	DefineUProperty(FScalableFloat, ObjectiveDistanceFromWall);
	DefineUProperty(FScalableFloat, SafeZonePhaseToStartObjectiveDestructionTimer);
	DefineUProperty(FScalableFloat, ObjectiveDestructionTimer);
	DefineUProperty(FScalableFloat, ObjectiveZOffset);
	DefineUProperty(FScalableFloat, ObjectiveHealthTiers);
	DefineUProperty(FScalableFloat, WallGravity);
	DefineUProperty(FScalableFloat, SafeZonePhaseWhenToBringDownWall);
	DefineUProperty(FScalableFloat, TimeToBringDownWall);
	DefineUProperty(FScalableFloat, RespawnCameraDistance);
	DefineUProperty(FScalableFloat, MinRespawnDistanceFromCenter);
	DefineUProperty(FScalableFloat, MaxRespawnDistanceFromCenter);
	DefineUProperty(FScalableFloat, MinRespawnDistanceFromGround);
	DefineUProperty(FScalableFloat, MaxRespawnDistanceFromGround);
	DefineUProperty(FScalableFloat, RespawnDirectionDeviation);
	DefineUProperty(FScalableFloat, SafeZonePhaseToTurnOffRespawning);
	DefineUProperty(FScalableFloat, PercentAmmoToDrop);
	DefineUProperty(FScalableFloat, AmmoDespawnTime);
	DefineUProperty(FScalableFloat, AmmoStormDespawnTime);
	DefineUProperty(TArray<FItemsToSpawn>, ItemsToDrop);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
	DefineUProperty(FAthenaGameMessageData, GameMsg_WallComingDown);
	DefineUProperty(FAthenaGameMessageData, GameMsg_WallDown);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Objective_Destroyed_Friendly);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Objective_Destroyed_Enemy);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawnsWarning_Friendly);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawns_Friendly);
	DefineUProperty(FAthenaGameMessageData, GameMsg_ObjectiveDamageState_Friendly);
public:
	void CheckHealthThreshold(uint8 TeamNum);

	void OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2, int32 EventParam3);

	static inline void (*BeginPlayOG)(AFortAthenaMutator_Barrier* This);
	static void BeginPlay(AFortAthenaMutator_Barrier* This);

	void OnGamePhaseStepChanged(EAthenaGamePhaseStep GamePhaseStep);
	static inline void (*execOnGamePhaseStepChangedOG)(AFortAthenaMutator_Barrier* Context, FFrame& Stack);
	static void execOnGamePhaseStepChanged(AFortAthenaMutator_Barrier* Context, FFrame& Stack);

	void SpawnBarrier(const FVector* WallStart, const FVector* WallEnd);

	void SpawnModeObjectives();

	void SetupTeamStates();

	void SpawnObjectiveActor(TSubclassOf<AAthenaBarrierFlag> InActorClass, FVector InActorLocation, FRotator InActorRotation, FBarrierTeamState* TeamState);

	static void Hook();
};
