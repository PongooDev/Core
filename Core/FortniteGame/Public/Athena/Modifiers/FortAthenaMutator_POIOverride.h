#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_POIOverride : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_POIOverride);

	DefineUProperty(TArray<FMapLocation>, MapLocations);
};
