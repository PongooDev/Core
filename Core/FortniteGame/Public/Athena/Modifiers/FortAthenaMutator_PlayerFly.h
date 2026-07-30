#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_PlayerFly : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PlayerFly);

	DefineUProperty(bool, bFlyEnabled);
};
