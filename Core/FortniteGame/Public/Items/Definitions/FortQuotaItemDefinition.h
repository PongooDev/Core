#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortQuotaItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortQuotaItemDefinition);

	DefineUProperty(float, StartingValue);
	DefineUProperty(int32, MaximumValue);
	DefineUProperty(float, UnitsPerMinuteRechargeRate);
	DefineUProperty(int32, RechargeDelayMinutes);
};
