#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

struct FFortSafeZoneDefinition {
public:
	DefineUnrealStruct(FFortSafeZoneDefinition);

	DefineStructProperty(FScalableFloat, Radius);
	DefineStructProperty(FScalableFloat, RejectRadius);
	DefineStructProperty(FScalableFloat, RejectOuterDistance);
	DefineStructProperty(FScalableFloat, WaitTime);
	DefineStructProperty(FScalableFloat, ShrinkTime);
	DefineStructProperty(FScalableFloat, FiftyFiftyWaitTimePlayerRatioScale);
	DefineStructProperty(FScalableFloat, FiftyFiftyShrinkTimePlayerRatioScale);
};