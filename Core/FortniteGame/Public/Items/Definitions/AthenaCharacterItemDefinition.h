#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UAthenaBackpackItemDefinition;
class UFortHeroType;

class UAthenaCharacterItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaCharacterItemDefinition);

	DefineUProperty(UFortHeroType*, HeroDefinition);
	DefineUProperty(UAthenaBackpackItemDefinition*, DefaultBackpack);
};
