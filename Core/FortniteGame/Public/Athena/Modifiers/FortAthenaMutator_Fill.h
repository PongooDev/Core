#pragma once
#include "pch.h"

#include "FortniteGame/Public/Athena/Modifiers/MutatorTypes.h"

#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_Fill : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Fill);

	DefineUProperty(TSubclassOf<AAthenaFillFloor>, FloorActorClass);
	DefineUProperty(FScalableFloat, SpawnHeight);
	DefineUProperty(FScalableFloat, ZKillOffset);
	DefineUProperty(FScalableFloat, ZMovementToleranceOffset);
	DefineUProperty(FScalableFloat, BuildableOffset);
	DefineUProperty(FScalableFloat, CanBuildOnLava);
	DefineUProperty(AAthenaFillFloor*, LavaFloor);
	DefineUProperty(FAthenaGameMessageData, GameMsg_LavaMoving);
};
