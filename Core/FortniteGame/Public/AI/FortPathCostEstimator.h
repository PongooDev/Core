#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AActor;
class AFortNavigationGraph;

class UFortPathCostEstimator : public UObject {
public:
	DefineUnrealClass(UFortPathCostEstimator);

	DefineUProperty(AActor*, GoalActor);
	DefineUProperty(AFortNavigationGraph*, NavGraph);
};
