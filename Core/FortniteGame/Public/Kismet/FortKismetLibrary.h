#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"

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

class UFortKismetLibrary : public UBlueprintFunctionLibrary {
public:
	DefineUnrealClass(UFortKismetLibrary);

	static UFortResourceItemDefinition* K2_GetResourceItemDefinition(const uint8 ResourceType);

	static EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		UClass* ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment);

	static EFortStructuralGridQueryResults CanPlaceBuildableClassInStructuralGrid(
		UObject* WorldContextObject,
		TSubclassOf<ABuildingSMActor> ClassToBuild,
		FVector WorldLocation,
		FRotator WorldRotation,
		bool bMirrored,
		TArray<ABuildingActor*>* ExistingBuildings,
		EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment);
	
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
		TArray<FFortItemEntry>& OutLootToDrop,
		FName TierGroupName,
		int32 WorldLevel,
		int32 ForcedLootTier
	);
	static void execPickLootDrops(UObject* Object, FFrame& Stack, bool* Result);

	static void Hook() {
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld", execK2_SpawnPickupInWorld);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.GiveItemToInventoryOwner", execGiveItemToInventoryOwner);
		ExecHook("Function /Script/FortniteGame.FortKismetLibrary.PickLootDrops", execPickLootDrops);

		Log("Hooked UFortKismetLibrary");
	}
};