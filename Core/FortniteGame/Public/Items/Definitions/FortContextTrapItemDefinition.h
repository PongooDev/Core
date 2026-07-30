#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Weapons/FortTrapItemDefinition.h"

class UFortContextTrapItemDefinition : public UFortTrapItemDefinition {
public:
	DefineUnrealClass(UFortContextTrapItemDefinition);

	DefineUProperty(UFortTrapItemDefinition*, FloorTrap);
	DefineUProperty(UFortTrapItemDefinition*, CeilingTrap);
	DefineUProperty(UFortTrapItemDefinition*, WallTrap);
};
