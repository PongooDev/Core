#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_MovementSpeed : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_MovementSpeed);

	DefineUProperty(float, MovementSpeed);
};
