#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
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
