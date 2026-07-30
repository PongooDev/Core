#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_NamePlate : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_NamePlate);

	DefineUProperty(EIndicatorDisplayMode, DisplayMode);
};
