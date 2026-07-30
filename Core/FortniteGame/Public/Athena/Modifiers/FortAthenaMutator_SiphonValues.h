#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_SiphonValues : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SiphonValues);

	DefineUProperty(float, HealthSiphonValue);
	DefineUProperty(float, WoodSiphonValue);
	DefineUProperty(float, StoneSiphonValue);
	DefineUProperty(float, MetalSiphonValue);
};
