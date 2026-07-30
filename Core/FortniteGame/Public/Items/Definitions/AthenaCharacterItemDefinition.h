#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UAthenaBackpackItemDefinition;
class UFortHeroType;

class UAthenaCharacterItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaCharacterItemDefinition);

	DefineUProperty(UFortHeroType*, HeroDefinition);
	DefineUProperty(UAthenaBackpackItemDefinition*, DefaultBackpack);
};
