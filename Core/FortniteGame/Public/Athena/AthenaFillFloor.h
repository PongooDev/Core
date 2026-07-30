#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

struct FFillFloorPositionData {
public:
	DefineUnrealStruct(FFillFloorPositionData);

	DefineStructProperty(FScalableFloat, MoveTime);
	DefineStructProperty(FScalableFloat, Height);
	DefineStructProperty(FScalableFloat, WaitTime);
public:
	uint8 Padding[0x60];
};

class AAthenaFillFloor : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaFillFloor);

	DefineUProperty(int32, StepIndex);
	DefineUProperty(float, FloorZ);
	DefineUProperty(FScalableFloat, InitialDelay);
	DefineUProperty(TArray<FFillFloorPositionData>, FloorPositionData);
	DefineUProperty(TArray<float>, EventHeights);
	DefineUProperty(FScalableFloat, MovementTimerUpdatePeriod);
	DefineUProperty(float, FloorMovementSpeed);
	DefineUProperty(bool, bIsMoving);
};
