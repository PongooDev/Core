#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

class EGoosePickupState {
public:
	DefineUnrealEnum(EGoosePickupState);

	DefineEnumProperty(None);
	DefineEnumProperty(Falling);
	DefineEnumProperty(Deployed);
	DefineEnumProperty(ImpactedByPlane);
	DefineEnumProperty(DeployTimerUp);
};

class AAthenaGoosePickup : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AAthenaGoosePickup);

	DefineUProperty(EGoosePickupState, PickupState);
	DefineUProperty(bool, bIsDespawning);
	DefineUProperty(FScalableFloat, DeployedLifetime);
	DefineUProperty(FScalableFloat, DespawnWarningTime);
	DefineUProperty(FScalableFloat, InStormLifetime);
	DefineUProperty(FScalableFloat, DropGravityScale);
	DefineUProperty(int32, DisplayIndex);
	DefineUProperty(FName, ForcedLootTableName);
	DefineUProperty(float, DeployHeight);
};
