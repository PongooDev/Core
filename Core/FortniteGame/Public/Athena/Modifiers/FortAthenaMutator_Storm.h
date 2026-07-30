#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Storm : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Storm);

	DefineUProperty(FScalableFloat, SafeZoneFinalDestinationX);
	DefineUProperty(FScalableFloat, SafeZoneFinalDestinationY);
};
