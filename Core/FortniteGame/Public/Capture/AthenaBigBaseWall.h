#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EBarrierState {
public:
	DefineUnrealEnum(EBarrierState);

	DefineEnumProperty(BarrierUp);
	DefineEnumProperty(BarrierComingDown);
	DefineEnumProperty(BarrierDown);
};

class AAthenaBigBaseWall : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaBigBaseWall);

	DefineUProperty(float, WallGravity);
	DefineUProperty(float, TimeUntilWallComesDown);
	DefineUProperty(bool, bResetBool);
	DefineUProperty(EBarrierState, BarrierState);
	DefineUProperty(FScalableFloat, ZKillLevel);
public:
	void OnRep_BarrierState();

	void OnRep_ResetBool();

	void OnRep_TimeUntilWallComesDown();

	void OnRep_WallGravity();

	static AAthenaBigBaseWall* CreateWall(UWorld* World, TSubclassOf<AAthenaBigBaseWall> BigBaseWallClass, const FVector& MidlineStart, const FVector& MidlineEnd, float ZLevel);
};
