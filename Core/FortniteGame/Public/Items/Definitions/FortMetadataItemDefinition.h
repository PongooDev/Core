#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"

class UFortMetadataItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortMetadataItemDefinition);

	DefineUProperty(int32, MinLevel);
	DefineUProperty(int32, MaxLevel);
};
