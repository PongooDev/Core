#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_SupplyDrop : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SupplyDrop);

	DefineUProperty(FScalableFloat, SpawnFailureLockoutTime);
	DefineUProperty(UEnvQuery*, BasePlacementQuery);
	DefineUProperty(float, BaseQueryingAttemptIntervalTimeSeconds);
	DefineUProperty(TArray<FFortSupplyDropMutatorData>, SafeZoneMutatorData);
	DefineUProperty(TArray<FVector>, BaseQueryResults);
	DefineUProperty(TSoftObjectPtr<UEnvQuery>, SupplyDropRemovalQuery);
	DefineUProperty(float, SupplyDropRemovalQueryInterval);
};
