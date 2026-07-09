#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class APlayerCameraManager : public AActor {
public:
	DefineUnrealClass(APlayerCameraManager);

	DefineUProperty(float, ViewRollMin);
	DefineUProperty(float, ViewRollMax);
};