#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_SpawningPolicyBase : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SpawningPolicyBase);

	DefineUProperty(TArray<FObjectiveSpecialActorContainer>, SpecialActorIDList);
	DefineUProperty(int32, CachedSpecialActorIdx);
	DefineUProperty(float, SpawnFailureLockoutTime);
	DefineUProperty(EAthenaGamePhase, GamePhaseToStartSpawning);
	DefineUProperty(UFortAthenaMutator_SpawningPolicyData*, ItemDataRemovalQueryPending);
	DefineUProperty(FScalableFloat, MinRespawnTime);
	DefineUProperty(FScalableFloat, MaxRespawnTime);
	DefineBitfieldUProperty(bShouldCenterGroundCheckAtFoundLocation);
	DefineBitfieldUProperty(bShouldMaintainItemCount);
	DefineBitfieldUProperty(bAllowedDespawnToMaintainItemCount);
};
