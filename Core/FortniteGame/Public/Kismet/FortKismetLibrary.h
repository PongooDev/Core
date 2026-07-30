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
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"


#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Abilities/FortAbilitySet.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"

class ABuildingActor;
class ABuildingSMActor;
class IFortInventoryOwnerInterface;
class UFortWorldItem;
class AFortAIGoalManager;
class AFortPlayerPawn;
class AFortAIDirector;
class AFortPickup;
class AFortPawn;
class UFortResourceItemDefinition;
class IAbilitySystemInterface;
class AFortPlayerController;
class UFortItemDefinition;
class UFortWorldItemDefinition;
class FFortBaseWeaponStats;

class UFortKismetLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UFortKismetLibrary);

	static UFortResourceItemDefinition* K2_GetResourceItemDefinition(const uint8 ResourceType);

	static uint8 CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		UClass* ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		uint8* MarkerOptionalAdjustment);

	static uint8 CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		TSubclassOf<ABuildingSMActor> ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		uint8* MarkerOptionalAdjustment);
	
	static FFortAbilitySetHandle EquipFortAbilitySet(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor, UFortAbilitySet* AbilitySet, UObject* OverrideSourceObject);

	static AFortPickup* K2_SpawnPickupInWorld(
		UObject* WorldContextObject,
		UFortItemDefinition* ItemDefinition,
		int NumberToSpawn,
		FVector Position,
		FVector Direction,
		int32 OverrideMaxStackCount,
		bool bToss,
		bool bRandomRotation,
		bool bBlockedFromAutoPickup,
		int32 PickupInstigatorHandle,
		EFortPickupSourceTypeFlag SourceType,
		EFortPickupSpawnSource Source,
		AFortPlayerController* OptionalOwnerPC,
		bool bPickupOnlyRelevantToOwner
	);
	static void execK2_SpawnPickupInWorld(UObject* Object, FFrame& Stack, AFortPickup** Result);

	static UFortWorldItem* GiveItemToInventoryOwner(
		TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner,
		UFortWorldItemDefinition* ItemDefinition,
		FGuid& ItemVariantGuid,
		int NumberToGive
	);
	static void execGiveItemToInventoryOwner(UObject* Object, FFrame& Stack, UFortWorldItem** Result);

	static bool PickLootDrops(
		UObject* WorldContextObject,
		TArray<FFortItemEntry>* OutLootToDrop,
		FName TierGroupName,
		int32 WorldLevel,
		int32 ForcedLootTier
	);
	static void execPickLootDrops(UObject* Object, FFrame& Stack, bool* Result);

	static AFortAIGoalManager* GetAIGoalManager(UObject* WorldContextObject);
	static void execGetAIGoalManager(UObject* Object, FFrame& Stack, AFortAIGoalManager** Result);

	static void K2_GiveItemToAllPlayers(
		UObject* WorldContextObject,
		UFortWorldItemDefinition* ItemDefinition,
		FGuid& ItemVariantGuid,
		int NumberToGive
	);
	static void execK2_GiveItemToAllPlayers(UObject* Object, FFrame& Stack);

	static AFortAIDirector* GetAIDirector(UObject* WorldContextObject);
	static void execGetAIDirector(UObject* Object, FFrame& Stack, AFortAIDirector** Result);

	static void K2_RemoveItemFromAllPlayers(
		UObject* WorldContextObject,
		UFortWorldItemDefinition* ItemDefinition,
		FGuid* ItemVariantGuid,
		int AmountToRemove
	);
	static void execK2_RemoveItemFromAllPlayers(UObject* Object, FFrame& Stack);

	static int32 K2_GetItemQuantityOnPlayer(
		AFortPlayerController* PlayerController,
		UFortItemDefinition* ItemDefinition
	);
	static void execK2_GetItemQuantityOnPlayer(UObject* Object, FFrame& Stack, int32* Result);

	static int32 K2_RemoveItemFromPlayer(
		AFortPlayerController* PlayerController,
		UFortItemDefinition* ItemDefinition,
		int32 AmountToRemove,
		bool bForceRemoval
	);
	static void execK2_RemoveItemFromPlayer(UObject* Object, FFrame& Stack, int32* Result);

	static int32 K2_RemoveItemFromPlayerByGuid(
		AFortPlayerController* PlayerController,
		FGuid ItemGuid,
		int32 AmountToRemove,
		bool bForceRemoval
	);
	static void execK2_RemoveItemFromPlayerByGuid(UObject* Object, FFrame& Stack, int32* Result);

	static bool GetWeaponStatsRow(const FDataTableRowHandle& DataTableRowHandle, FFortBaseWeaponStats* OutRow);

	static FVector FindStaticGroundLocationAt(UWorld* World, const FVector& InLocation, const AActor* IgnoreActor, float TraceStartZ, float TraceEndZ);

	static ABuildingGameplayActor* SpawnBuildingGameplayActor(TSubclassOf<class ABuildingGameplayActor> BGAClass, const FTransform& Transform, class AActor* Instigator);

	static bool GetSafeZoneLocation(UObject* WorldContextObject, int32 SafeZoneIndex, FVector* OutLocation);

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld", execK2_SpawnPickupInWorld);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.GiveItemToInventoryOwner", execGiveItemToInventoryOwner);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.PickLootDrops", execPickLootDrops);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.GetAIGoalManager", execGetAIGoalManager);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_GiveItemToAllPlayers", execK2_GiveItemToAllPlayers);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.GetAIDirector", execGetAIDirector);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_RemoveItemFromAllPlayers", execK2_RemoveItemFromAllPlayers);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_GetItemQuantityOnPlayer", execK2_GetItemQuantityOnPlayer);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_RemoveItemFromPlayer", execK2_RemoveItemFromPlayer);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_RemoveItemFromPlayerByGuid", execK2_RemoveItemFromPlayerByGuid);

		Log("Hooked UFortKismetLibrary");
	}
};