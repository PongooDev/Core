#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

class AAthenaFillFloor : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaFillFloor);

	DefineUProperty(int32, StepIndex);
	DefineUProperty(float, FloorZ);
	DefineUProperty(FScalableFloat, InitialDelay);
	DefineUProperty(TArray<float>, EventHeights);
	DefineUProperty(FScalableFloat, MovementTimerUpdatePeriod);
	DefineUProperty(float, FloorMovementSpeed);
	DefineUProperty(bool, bIsMoving);
};
