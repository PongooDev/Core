#pragma once
#include "pch.h"

#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"

class UFortWeaponItemDefinition;
class UMaterialInterface;
class UStaticMesh;

struct FGooseDisplayData {
public:
	DefineUnrealStruct(FGooseDisplayData);

	DefineStructProperty(FName, LootTableName);
	DefineStructProperty(UStaticMesh*, StaticMesh);
	DefineStructProperty(TArray<UMaterialInterface*>, MaterialsToSwap);
public:
	uint8 Padding[0x20];
};

struct FGooseWeaponTierData {
public:
	DefineUnrealStruct(FGooseWeaponTierData);

	DefineStructProperty(TArray<UFortWeaponItemDefinition*>, WeaponItemDefinitions);
public:
	uint8 Padding[0x10];
};

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
	DefineUProperty(TArray<FGooseWeaponTierData>, WeaponTierData);
	DefineUProperty(FName, ForcedLootTableName);
	DefineUProperty(TArray<FGooseDisplayData>, DisplayData);
	DefineUProperty(float, DeployHeight);
};
