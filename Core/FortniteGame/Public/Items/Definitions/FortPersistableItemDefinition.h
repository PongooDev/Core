#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"

class UFortPersistableItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortPersistableItemDefinition);
};
