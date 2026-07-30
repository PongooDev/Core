#pragma once
#include "pch.h"

#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"

class UMaterialInstanceDynamic;
class UFortSlateHUDIndicator;

class AAthenaCapturePoint : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaCapturePoint);

	DefineUProperty(UMaterialInstanceDynamic*, HUDIndicatorMID);
	DefineUProperty(int32, IconMaterialIndexParameter);
	DefineUProperty(bool, bPermanentShutdown);
	DefineUProperty(float, ShutdownTime);
	DefineUProperty(UFortSlateHUDIndicator*, HUDIndicatorRef);
	DefineUProperty(FVector2D, CurrHUDIndicatorSize);
	DefineUProperty(UMaterialInstanceDynamic*, CapturePointMID_Neutral);
	DefineUProperty(UMaterialInstanceDynamic*, CapturePointMID_AllyCaptured);
};
