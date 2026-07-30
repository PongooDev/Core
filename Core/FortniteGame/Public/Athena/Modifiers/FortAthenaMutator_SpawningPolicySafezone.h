#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_SpawningPolicyBase.h"

class AFortAthenaMutator_SpawningPolicySafezone : public AFortAthenaMutator_SpawningPolicyBase {
public:
	DefineUnrealClass(AFortAthenaMutator_SpawningPolicySafezone);

	DefineUProperty(TArray<UFortAthenaMutator_SpawningPolicyData*>, ItemSpawningMutatorPerSafeZonePhase);
	DefineUProperty(float, BaseQueryingAttemptIntervalTimeSeconds);
	DefineUProperty(TSoftObjectPtr<UEnvQuery>, BasePlacementQuery);
};
