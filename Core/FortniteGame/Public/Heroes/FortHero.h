#pragma once
#include "pch.h"

#include "FortniteGame/Public/Heroes/FortWorker.h"

class UCustomCharacterPart;

class UFortHero : public UFortWorker {
public:
	DefineUnrealClass(UFortHero);

	DefineUProperty(TArray<UCustomCharacterPart*>, CharacterParts);
};