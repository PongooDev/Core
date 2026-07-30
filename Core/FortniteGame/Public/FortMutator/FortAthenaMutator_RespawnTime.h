#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_RespawnTime : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_RespawnTime);

	DefineUProperty(float, RespawnTime);
};
