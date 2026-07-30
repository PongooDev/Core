#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_PickaxeDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PickaxeDamage);

	DefineUProperty(bool, PickaxeInstantDestroy);
};
