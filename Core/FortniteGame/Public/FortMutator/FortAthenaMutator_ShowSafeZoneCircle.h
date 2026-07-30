#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_ShowSafeZoneCircle : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_ShowSafeZoneCircle);

	DefineUProperty(bool, bShowSafeZoneCircle);
};
