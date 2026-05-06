#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"

class UFortPlaylist : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortPlaylist);

	DefineUProperty(int32, PlaylistId);
	DefineUProperty(FName, PlaylistName);

	DefineUProperty(float, GarbageCollectionFrequency);

	DefineUProperty(int32, MaxPlayers);

	DefineUProperty(int32, MaxSquadSize);

	DefineUProperty(TArray<TSoftObjectPtr<UWorld>>, AdditionalLevels);
	DefineUProperty(TArray<TSoftObjectPtr<UWorld>>, AdditionalLevelsServerOnly);

	DefineUProperty(TSoftObjectPtr<UDataTable>, LootTierData);
	DefineUProperty(TSoftObjectPtr<UDataTable>, LootPackages);
};