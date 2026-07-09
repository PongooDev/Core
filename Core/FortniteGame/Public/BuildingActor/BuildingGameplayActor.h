#pragma once
#include "pch.h"

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

#include "BuildingActor.h"

class AFortPickup;
class UFortWorldItemDefinition;
class AFortPawn;

class ABuildingGameplayActor : public ABuildingActor {
public:
	DefineUnrealClass(ABuildingGameplayActor);
};

class AFortAthenaSupplyDrop : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AFortAthenaSupplyDrop);
public:
	AFortPickup* SpawnPickup(UFortWorldItemDefinition* ItemDefinition, int32 NumberToSpawn, AFortPawn* TriggeringPawn, FVector& Position, FVector& Direction);
	static void execSpawnPickup(AFortAthenaSupplyDrop* Context, FFrame& Stack, AFortPickup** Result);

	FVector FindGroundLocationAt(const FVector& InLocation);

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.FortAthenaSupplyDrop.SpawnPickup", execSpawnPickup);

		Log("Hooked ABuildingGameplayActor");
	}
};

class AB_SupplyDropPlacement_C : public ABuildingGameplayActor {
public:
	DefineUnrealClass(AB_SupplyDropPlacement_C);

	DefineUProperty(FVector, LootSpawnOffset);
	DefineUProperty(TArray<FName>, LootTableNames);
	DefineUProperty(bool, bLooted);
};