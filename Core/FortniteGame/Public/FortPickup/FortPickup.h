#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/ProjectileMovementComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortEnums.h"
#include "FortPickupLocationData.h"

class UFortItemDefinition;
class AFortPawn;
class AFortPickupEffect;
class IFortInventoryOwnerInterface;
class AFortPlayerPawn;

class AFortPickup : public AActor {
public:
	DefineUnrealClass(AFortPickup);

	DefineUProperty(bool, bRandomRotation);

	DefineUProperty(bool, bTossedFromContainer);

	DefineUProperty(FFortItemEntry, PrimaryPickupItemEntry);

	DefineUProperty(TArray<FFortItemEntry>, MultiItemPickupEntries);

	DefineUProperty(UProjectileMovementComponent*, MovementComponent);

	DefineUProperty(AFortPawn*, PawnWhoDroppedPickup);

	DefineUProperty(bool, bPickedUp);

	DefineUProperty(FFortPickupLocationData, PickupLocationData);

	DefineUProperty(TWeakObjectPtr<AFortPickupEffect>, PickupEffectBlueprint);
public:
	void OnRep_PrimaryPickupItemEntry();

	void OnRep_TossedFromContainer();

	void TossPickup(const FVector& FinalLocation, AFortPawn* ItemOwner, int32 OverrideMaxStackCount, bool bToss, bool bShouldCombinePickupsWhenTossCompletes, const uint8 InPickupSourceTypeFlags, const uint8 InPickupSpawnSource);

	void OnRep_bPickedUp();

	void OnRep_PickupLocationData();

	void ForceFinishedTargetSpline();

	void SetPickupTarget(AFortPawn* PickupTarget, float InFlyTime, FVector InStartDirection, bool bPlayPickupSound);

	void UpdateSpecialActorStat(ESpecialActorStatType InStat, double InValue, float InLogicCondition);

	void SetupForMovementCompToss();

	void SetPickupItems(
		FFortItemEntry* PrimaryEntry,
		TArray<FFortItemEntry>* AdditionalEntries,
		uint8 SourceTypeFlags,
		bool bInSplitOnPickup,
		uint8 SpawnSource
	);

	static inline void (*GivePickupToOG)(AFortPickup* This, IFortInventoryOwnerInterface* InventoryOwner, bool DestroyAfterPickup);
	static void GivePickupTo(AFortPickup* This, IFortInventoryOwnerInterface* InventoryOwner, bool DestroyAfterPickup);

	bool CheckForRePickup(AFortPlayerPawn* FortPlayerPawn);

	static void Hook() {
		HookEveryVTableIdx(
			AFortPickup::StaticClass(),
			Finder::FindAFortPickup_GivePickupToVFT(),
			GivePickupTo,
			(LPVOID*)&GivePickupToOG
		);

		Log("AFortPickup Hooked");
	}
};