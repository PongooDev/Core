#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortPrerollDataItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortPrerollDataItemDefinition);

	DefineUProperty(float, StreakbreakerRefundMultiplier);
};
