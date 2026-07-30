#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/FortEnums.h"

class AAthenaCapturePoint;
class AFortTeamInfoAthena;

struct FFortPieSliceSpawnData {
public:
	DefineUnrealStruct(FFortPieSliceSpawnData);

	DefineStructProperty(FScalableFloat, SpawnDirection);
	DefineStructProperty(FScalableFloat, SpawnDirectionDeviation);
	DefineStructProperty(FScalableFloat, MinSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, MaxSpawnDistanceFromCenter);
public:
	uint8 Padding[0x80];
};

struct FControlPointSpawnData {
public:
	DefineUnrealStruct(FControlPointSpawnData);

	DefineStructProperty(FScalableFloat, SpawnDirection);
	DefineStructProperty(FScalableFloat, SpawnDirectionDeviation);
	DefineStructProperty(FScalableFloat, MinSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, MaxSpawnDistanceFromCenter);
	DefineStructProperty(FScalableFloat, SpawnDelayTime);
	DefineStructProperty(FScalableFloat, SafeZonePhaseWhenToSpawn);
	DefineStructProperty(FScalableFloat, SafeZonePhaseWhereToSpawn);
	DefineStructProperty(FScalableFloat, EnableDelayTime);
	DefineStructProperty(FScalableFloat, EnableInSafeZonePhase);
	DefineStructProperty(FScalableFloat, DisableDelayTime);
	DefineStructProperty(FScalableFloat, DisableInSafeZonePhase);
	DefineStructProperty(FScalableFloat, PointsEarnedPerSecond);
	DefineStructProperty(FScalableFloat, BonusPointsEarnedPerSecond);
	DefineStructProperty(FScalableFloat, bIgnoreForOrderMessaging);
	DefineStructProperty(int32, IconMaterialIndex);
	DefineStructProperty(bool, bAlwaysInPlay);
public:
	uint8 Padding[0x1C8];
};

struct FControlPointInstanceData {
public:
	DefineUnrealStruct(FControlPointInstanceData);

	DefineStructProperty(AAthenaCapturePoint*, ControlPoint);
	DefineStructProperty(EControlPointState, ControlPointState);
	DefineStructProperty(int32, SpawnDataIdx);
	DefineStructProperty(float, SpawnTime);
	DefineStructProperty(float, EnableTime);
	DefineStructProperty(float, DisableTime);
	DefineStructProperty(uint8, PrevOwningTeam);
	DefineStructProperty(AFortTeamInfoAthena*, CachedOwningTeamInfo);
	DefineStructProperty(float, PointAccrualTime);
	DefineStructProperty(float, PointsRemainder);
	DefineStructProperty(float, BonusPointAccrualTime);
	DefineStructProperty(float, BonusPointsRemainder);
	DefineStructProperty(float, CachedPointAccrualValue);
	DefineStructProperty(float, CachedBonusPointAccrualValue);
	DefineStructProperty(bool, bPointFinished);
	DefineStructProperty(int32, CachedSafeZonePhaseWhenToSpawn);
	DefineStructProperty(bool, bIgnoreForOrderMessaging);
	DefineStructProperty(bool, bAlwaysInPlay);
	DefineStructProperty(float, TimeOfShutdown);
public:
	uint8 Padding[0x50];
};

struct FControlPointAssetData {
public:
	DefineUnrealStruct(FControlPointAssetData);

	DefineStructProperty(TSubclassOf<AAthenaCapturePoint>, CapturePointClass);
	DefineStructProperty(FScalableFloat, SpawnDistanceFromGround);
	DefineStructProperty(FVector2D, MiniMapIconScale);
	DefineStructProperty(FVector2D, CompassIconScale);
public:
	uint8 Padding[0x38];
};

struct FFortRespawnLogicData {
public:
	DefineUnrealStruct(FFortRespawnLogicData);

	DefineStructProperty(FScalableFloat, DirectionDeviation);
	DefineStructProperty(FScalableFloat, MinDistFromCenterPercent);
	DefineStructProperty(FScalableFloat, MaxDistFromCenterPercent);
	DefineStructProperty(FScalableFloat, MinHeightFromGround);
	DefineStructProperty(FScalableFloat, MinHeightFromZero);
	DefineStructProperty(FScalableFloat, CameraDistance);
	DefineStructProperty(FScalableFloat, RespawnTraceHeight);
public:
	uint8 Padding[0xE0];
};
