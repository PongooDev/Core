#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/Athena/FortSafeZoneDefinition.h"

class ABuildingItemCollectorActor;
class AFortAthenaSupplyDrop;
class UFortSupplyDropInfo;
class AFortAthenaExitCraftSpawner;
class UFortAthenaExitCraftInfo;

class AFortAthenaMapInfo : public AActor {
public:
	DefineUnrealClass(AFortAthenaMapInfo);

	DefineUProperty(TSubclassOf<ABuildingItemCollectorActor>, VendingMachineClass);
	DefineUProperty(FScalableFloat, VendingMachineRarityCount);
	DefineUProperty(TSubclassOf<AFortAthenaSupplyDrop>, LlamaClass);
	DefineUProperty(FScalableFloat, LlamaQuantityMin);
	DefineUProperty(FScalableFloat, LlamaQuantityMax);
	DefineUProperty(FFortSafeZoneDefinition, SafeZoneDefinition);
	DefineUProperty(TArray<FFortSafeZoneDefinition>, SafeZoneDefinitions);
	DefineUProperty(TArray<UFortSupplyDropInfo*>, SupplyDropInfoList);
	DefineUProperty(TSubclassOf<AFortAthenaSupplyDrop>, SupplyDropClass);
public:
	void SpawnLlamas();

	FVector PickSupplyDropLocation(FVector& CenterLocation, float MaxRadius, bool bAvoidWater, float TraceStartZ, float TraceEndZ);

	FVector GetMapCenter() const;

	static void Hook();
};