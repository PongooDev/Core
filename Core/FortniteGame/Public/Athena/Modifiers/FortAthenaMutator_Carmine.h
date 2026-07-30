#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Carmine : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Carmine);

	DefineUProperty(FGameplayTag, JimInventoryTag);
	DefineUProperty(FGameplayTag, JimPickupTag);
};
