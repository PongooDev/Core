#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

class UCustomCharacterPart : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UCustomCharacterPart);

	DefineUProperty(uint8, CharacterPartType);
};