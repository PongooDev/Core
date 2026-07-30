#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EDBNOMutatorType {
public:
	DefineUnrealEnum(EDBNOMutatorType);

	DefineEnumProperty(Default);
	DefineEnumProperty(On);
	DefineEnumProperty(Off);
	DefineEnumProperty(EDBNOMutatorType_MAX);
};

class AFortAthenaMutator_DBNOSetting : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_DBNOSetting);

	DefineUProperty(EDBNOMutatorType, DBNOSetting);
};
