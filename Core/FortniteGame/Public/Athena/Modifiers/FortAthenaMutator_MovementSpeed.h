#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_MovementSpeed : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_MovementSpeed);

	DefineUProperty(float, MovementSpeed);
};
