#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Carmine : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Carmine);

	DefineUProperty(FGameplayTag, JimInventoryTag);
	DefineUProperty(FGameplayTag, JimPickupTag);
};
