#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

class ESubGame {
public:
	DefineUnrealEnum(ESubGame);

	DefineEnumProperty(Campaign);
	DefineEnumProperty(Athena);
	DefineEnumProperty(Invalid);
};

class UFortGlobals : public UObject {
public:
	DefineUnrealClass(UFortGlobals);

	DefineUProperty(bool, bEnableDBNO);
};
