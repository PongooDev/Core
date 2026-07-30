#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EIndicatorDisplayMode {
public:
	DefineUnrealEnum(EIndicatorDisplayMode);

	DefineEnumProperty(Default);
	DefineEnumProperty(Always);
	DefineEnumProperty(Never);
	DefineEnumProperty(EIndicatorDisplayMode_MAX);
};

class AFortAthenaMutator_NamePlate : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_NamePlate);

	DefineUProperty(EIndicatorDisplayMode, DisplayMode);
};
