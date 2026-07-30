#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UCustomCharacterPart;

class UAthenaCharacterPartItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaCharacterPartItemDefinition);

	DefineUProperty(TArray<UCustomCharacterPart*>, CharacterParts);
};
