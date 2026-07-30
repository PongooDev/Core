#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortConsumableAccountItemDefinition.h"

class UFortAccountBuffCreditItemDefinition : public UFortConsumableAccountItemDefinition {
public:
	DefineUnrealClass(UFortAccountBuffCreditItemDefinition);

	DefineUProperty(int32, MinutesOfBuffTimeToGrant);
};
