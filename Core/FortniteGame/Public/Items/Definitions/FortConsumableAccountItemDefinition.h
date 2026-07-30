#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class EItemProfileType {
public:
	DefineUnrealEnum(EItemProfileType);

	DefineEnumProperty(Common);
	DefineEnumProperty(Campaign);
	DefineEnumProperty(Athena);
};

class UFortConsumableAccountItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortConsumableAccountItemDefinition);

	DefineUProperty(bool, bCanActivateOnSelf);
	DefineUProperty(bool, bCanActivateOnOthers);
	DefineUProperty(bool, bIsAutomaticallyConsumed);
	DefineUProperty(EItemProfileType, ProfileType);
};
