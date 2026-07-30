#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Storm : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Storm);

	DefineUProperty(FScalableFloat, SafeZoneFinalDestinationX);
	DefineUProperty(FScalableFloat, SafeZoneFinalDestinationY);
};
