#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortAwardDetector;

class UFortAwardItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAwardItemDefinition);

	DefineUProperty(bool, bSingleSharedDetector);
	DefineUProperty(bool, bCanEarnMultipleTimes);
	DefineUProperty(TSubclassOf<UFortAwardDetector>, DetectorClass);
};
