#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"

class UEnvQuery;

class UFortAthenaMutator_SpawningPolicyData : public UDataAsset {
public:
	DefineUnrealClass(UFortAthenaMutator_SpawningPolicyData);

	DefineUProperty(FScalableFloat, NumItemsToSpawn);
	DefineUProperty(TSoftObjectPtr<UEnvQuery>, ItemPlacementQuery);
	DefineUProperty(TSoftObjectPtr<UEnvQuery>, ItemRemovalQuery);
	DefineUProperty(float, RemovalQueryInterval);
};
