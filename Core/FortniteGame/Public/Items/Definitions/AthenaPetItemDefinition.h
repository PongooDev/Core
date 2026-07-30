#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/AthenaCosmeticItemDefinition.h"

class UFortPetStimuliBank;
class UFortTaggedSoundBank;

class UAthenaPetItemDefinition : public UAthenaCosmeticItemDefinition {
public:
	DefineUnrealClass(UAthenaPetItemDefinition);

	DefineUProperty(TArray<UFortPetStimuliBank*>, StimuliBanks);
	DefineUProperty(TSoftClassPtr<UClass>, PetPrefabClass);
	DefineUProperty(TSoftObjectPtr<UFortTaggedSoundBank>, PetSoundBank);
};
