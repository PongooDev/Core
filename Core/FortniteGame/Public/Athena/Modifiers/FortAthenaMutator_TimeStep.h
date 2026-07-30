#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_TimeStep : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TimeStep);

	DefineUProperty(float, TimeStep);
};
