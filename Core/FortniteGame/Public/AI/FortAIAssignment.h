#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class AFortAIController;
class UFortAIAssignmentSettings;
class UFortAIGoalProvider;

class UFortAIAssignment : public UObject {
public:
	DefineUnrealClass(UFortAIAssignment);

	DefineUProperty(UFortAIAssignmentSettings*, AssignmentSettings);
	DefineUProperty(UFortAIGoalProvider*, GoalProvider);
	DefineUProperty(TArray<AFortAIController*>, ControllersOnAssignment);
};
