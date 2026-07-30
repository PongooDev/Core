#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortAwardDetector;

class UFortAwardItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAwardItemDefinition);

	DefineUProperty(bool, bSingleSharedDetector);
	DefineUProperty(bool, bCanEarnMultipleTimes);
	DefineUProperty(TSubclassOf<UFortAwardDetector>, DetectorClass);
};
