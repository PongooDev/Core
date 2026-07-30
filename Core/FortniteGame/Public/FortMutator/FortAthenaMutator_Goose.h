#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Goose : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Goose);

	DefineUProperty(TArray<TSubclassOf<UGameplayEffect>>, PlaneStatusEffects);
	DefineUProperty(FScalableFloat, AircraftPhaseLength);
	DefineUProperty(FVector, PlaneSpawnLocation);
	DefineUProperty(FScalableFloat, InitialRadius);
	DefineUProperty(FScalableFloat, PlaneSpawnHeight);
	DefineUProperty(FScalableFloat, PlaneSkyCapOffset);
	DefineUProperty(FScalableFloat, PlaneSpawnPitch);
	DefineUProperty(FScalableFloat, InitialNumOfSleepingPlanes);
	DefineUProperty(FScalableFloat, PlaneSpawnStormRadiusMultiplier);
	DefineUProperty(FScalableFloat, GroundTestValue);
	DefineUProperty(FScalableFloat, DriverRespawnScalar);
	DefineUProperty(TSubclassOf<AFortAthenaVehicle>, VehicleClass);
	DefineUProperty(TSubclassOf<UGameplayEffect>, SiphonEffectForPlanes);
	DefineUProperty(FScalableFloat, NumberOfPlanesPerSquad);
	DefineUProperty(TArray<FPlaneLootTypeEntry>, LootTypeEntries);
	DefineUProperty(FScalableFloat, HowOftenToUpdatePickupCount);
	DefineUProperty(FName, DefaultLootTableName);
	DefineUProperty(FScalableFloat, DefaultDeployAltitude);
	DefineUProperty(FScalableFloat, DefaultPickupSpawnHeight);
	DefineUProperty(TSubclassOf<AAthenaGoosePickup>, PickupClass);
	DefineUProperty(FScalableFloat, MaxNumOfAlivePickups);
	DefineUProperty(TSubclassOf<AActor>, BoundsXYSplineClass);
	DefineUProperty(FPlaneSquadInfoArray, SquadInfoArray);
	DefineUProperty(TArray<AFortAthenaVehicle*>, SpawnedVehicles);
	DefineUProperty(TArray<AAthenaGoosePickup*>, SpawnedPickups);
};
