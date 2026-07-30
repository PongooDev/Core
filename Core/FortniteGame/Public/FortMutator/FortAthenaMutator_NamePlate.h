#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_NamePlate : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_NamePlate);

	DefineUProperty(EIndicatorDisplayMode, DisplayMode);
};
