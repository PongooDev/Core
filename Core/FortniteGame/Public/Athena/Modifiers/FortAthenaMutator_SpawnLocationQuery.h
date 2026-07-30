#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_SpawnLocationQuery : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SpawnLocationQuery);

	DefineUProperty(FGameplayTagQuery, LocationQuery);
};
