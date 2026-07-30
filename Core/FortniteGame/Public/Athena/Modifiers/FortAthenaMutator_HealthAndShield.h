#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EAthenaMutatorEvaluators {
public:
	DefineUnrealEnum(EAthenaMutatorEvaluators);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(ForceOverride);
	DefineEnumProperty(Add);
	DefineEnumProperty(Multiply);
	DefineEnumProperty(EAthenaMutatorEvaluators_MAX);
};

class AFortAthenaMutator_HealthAndShield : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_HealthAndShield);

	DefineUProperty(EAthenaMutatorEvaluators, NumericalMutatorOverride);
	DefineUProperty(float, MaxHealth);
	DefineUProperty(float, StartingHealth);
	DefineUProperty(float, MaxShield);
	DefineUProperty(float, StartingShield);
};
