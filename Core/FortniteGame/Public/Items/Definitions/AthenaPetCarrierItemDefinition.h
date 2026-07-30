#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaBackpackItemDefinition.h"

class UAthenaPetItemDefinition;

class UAthenaPetCarrierItemDefinition : public UAthenaBackpackItemDefinition {
public:
	DefineUnrealClass(UAthenaPetCarrierItemDefinition);

	DefineUProperty(UAthenaPetItemDefinition*, DefaultPet);
};
