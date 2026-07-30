#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_BlockBuilding : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_BlockBuilding);

	DefineUProperty(bool, bBlockBuilding);
};
