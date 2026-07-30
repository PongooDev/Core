#include "pch.h"
#include "FortniteGame/Public/Building/BGAConsumableSpawner.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Building/BuildingGameplayActor.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetSystemLibrary.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Building/BuildingGameplayActorConsumable.h"
#include "FortniteGame/Public/Items/Definitions/BGAConsumableWrapperItemDefinition.h"

bool ABGAConsumableSpawner::AttemptSpawn() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return false;
	}

	AFortGameStateZone* GameState = World->GameState->Cast<AFortGameStateZone>();
	if (!GameState) {
		return false;
	}

	if (AssociatedBuildingActor && AssociatedBuildingActor->bDestroyed) {
		return false;
	}

	TArray<FFortItemEntry> LootDrops;
	if (!SpawnLootTierGroup.IsNone()) {
		UFortKismetLibrary::PickLootDrops(World, &LootDrops, SpawnLootTierGroup, GameState->WorldLevel, -1);
	}

	TArray<FFortItemEntry>& Consumables = LootDrops.Num() > 0 ? LootDrops : ConsumablesToSpawn;

	bool bSpawnedAny = false;

	for (int32 i = 0; i < Consumables.Num(); i++) {
		FFortItemEntry& ItemEntry = Consumables.GetWithSize(i, FFortItemEntry::GetSize());
		if (!ItemEntry.ItemDefinition) {
			continue;
		}

		UClass* ConsumableActorClass = nullptr;
		UBGAConsumableWrapperItemDefinition* WrapperDef = ItemEntry.ItemDefinition->Cast<UBGAConsumableWrapperItemDefinition>();
		if (WrapperDef) {
			ConsumableActorClass = WrapperDef->ConsumableClass.Get();
		}

		if (ConsumableActorClass) {
			FVector SpawnLocation = K2_GetActorLocation();
			FRotator SpawnRotation = K2_GetActorRotation();

			ABuildingGameplayActorConsumable* ConsumableCDO = ConsumableActorClass->GetDefaultObj()->Cast<ABuildingGameplayActorConsumable>();
			if (ConsumableCDO && ConsumableCDO->bSpawnerCalculateRandomRotation) {
				SpawnRotation.Yaw = (float)(rand() % 360);
			}

			if (bAlignSpawnedActorsToSurface) {
				TArray<AActor*> ActorsToIgnore;
				if (AssociatedBuildingActor) {
					ActorsToIgnore.Add(AssociatedBuildingActor);
				}

				FHitResult Hit;
				if (UKismetSystemLibrary::LineTraceSingle(this, SpawnLocation, SpawnLocation - FVector(0.f, 0.f, 100000.f), 0, false, ActorsToIgnore, 0, &Hit, true)
					&& Hit.IsValidBlockingHit()) {
					SpawnLocation = Hit.ImpactPoint;
				}

				ActorsToIgnore.Free();
			}

			ABuildingActor* Consumable = World->SpawnActor(ConsumableActorClass, SpawnLocation, SpawnRotation)->Cast<ABuildingActor>();
			if (Consumable) {
				bSpawnedAny = true;
			}

			continue;
		}

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry.ItemDefinition,
			ItemEntry.Count,
			K2_GetActorLocation(),
			*FVector::Allocate(),
			-1,
			bAlignSpawnedActorsToSurface,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetOther(),
			EFortPickupSpawnSource::GetItemSpawner(),
			nullptr,
			false
		);

		if (Pickup) {
			bSpawnedAny = true;
		}
	}

	return bSpawnedAny;
}
