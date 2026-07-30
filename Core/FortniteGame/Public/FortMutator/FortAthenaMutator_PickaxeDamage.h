#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_PickaxeDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PickaxeDamage);

	DefineUProperty(bool, PickaxeInstantDestroy);
};
