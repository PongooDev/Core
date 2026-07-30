#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class EHomebaseNodeType {
public:
	DefineUnrealEnum(EHomebaseNodeType);

	DefineEnumProperty(Gadget);
	DefineEnumProperty(Utility);
	DefineEnumProperty(Hidden);
};

class UFortHomebaseNodeItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortHomebaseNodeItemDefinition);

	DefineUProperty(EHomebaseNodeType, DisplayType);
};
