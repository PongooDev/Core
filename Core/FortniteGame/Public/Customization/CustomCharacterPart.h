#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EFortCustomPartType {
public:
	DefineUnrealEnum(EFortCustomPartType);

	DefineEnumProperty(Head);
	DefineEnumProperty(Body);
	DefineEnumProperty(Hat);
	DefineEnumProperty(Backpack);
	DefineEnumProperty(Charm);
	DefineEnumProperty(Face);
	DefineEnumProperty(NumTypes);
};

class UCustomCharacterPart : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UCustomCharacterPart);

	DefineUProperty(uint8, CharacterPartType);
};
