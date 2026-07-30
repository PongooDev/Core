#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/AssetPtr.h"

class UCustomCharacterPart;

class UFortHeroSpecialization : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortHeroSpecialization);

	DefineUProperty(TArray<TSoftObjectPtr<UCustomCharacterPart>>, CharacterParts);
public:
	TArray<TAssetPtr<UCustomCharacterPart>>& GetCharacterPartsAssetPtr() {
		TArray<TSoftObjectPtr<UCustomCharacterPart>>& CharacterPartsSoftPtr = CharacterParts;
		return *reinterpret_cast<TArray<TAssetPtr<UCustomCharacterPart>>*>(&CharacterPartsSoftPtr);
	}
};