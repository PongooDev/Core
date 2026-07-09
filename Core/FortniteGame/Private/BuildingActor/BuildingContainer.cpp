#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

bool ABuildingContainer::SpawnLoot(ABuildingContainer* This, AFortPlayerPawn* PlayerPawn, uint8 InSourceTypeFlag, uint8 InSpawnSource) {
	if (!This) {
		Log("ABuildingContainer::SpawnLoot: This is null!");
		return false;
	}
	
	if (This->bAlreadySearched) {
		return false;
	}
	
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingContainer::SpawnLoot: World is null!");
		return false;
	}

	FVector ContainerLocation = This->K2_GetActorLocation();
	FVector LootSpawnLocation = This->LootSpawnLocation;
	FVector FinalSpawnLocation = ContainerLocation + (This->GetActorForwardVector() * LootSpawnLocation.X) +
		(This->GetActorRightVector() * LootSpawnLocation.Y) + (This->GetActorUpVector() * LootSpawnLocation.Z);
	FVector LootFinalLocation = ContainerLocation + (This->GetActorForwardVector() * This->LootFinalLocation.X) + (This->GetActorRightVector() * This->LootFinalLocation.Y) +
		(This->GetActorUpVector() * This->LootFinalLocation.Z);

	TArray<FFortItemEntry> LootDrops;
	bool bSuccess = UFortKismetLibrary::PickLootDrops(This, &LootDrops, This->SearchLootTierGroup, 0, This->ReplicatedLootTier);

	for (int i = 0; i < LootDrops.Num(); i++) {
		FFortItemEntry& ItemEntry = LootDrops.GetWithSize(i, FFortItemEntry::GetSize());
		if (!ItemEntry.ItemDefinition) {
			Log("ABuildingContainer::SpawnLoot: ItemDefinition is null for loot entry " + std::to_string(i));
			continue;
		}

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry.ItemDefinition,
			ItemEntry.Count,
			FinalSpawnLocation,
			*FVector::Allocate(),
			-1,
			true,
			true,
			false,
			-1,
			InSourceTypeFlag,
			InSpawnSource,
			nullptr,
			false
		);
		if (Pickup) {
			UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
			if (WeaponDef) {
				int32 Level = Pickup->PrimaryPickupItemEntry.Level;
				Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
				Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
				Pickup->PrimaryPickupItemEntry.bIsDirty = true;
				Pickup->PrimaryPickupItemEntry.ReplicationKey++;
				Pickup->OnRep_PrimaryPickupItemEntry();
			}
		}
	}

	This->bAlreadySearched = true;
	This->OnRep_bAlreadySearched();
	This->SearchBounceData.SearchAnimationCount++;
	This->BounceContainer();

	if (This->bDestroyContainerOnSearch) {
		This->K2_DestroyActor();
	}

	This->ForceNetUpdate();

	return bSuccess;
}

void ABuildingContainer::OnRep_bAlreadySearched()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_bAlreadySearched"));

	Call(Func);
}

void ABuildingContainer::BounceContainer()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"BounceContainer"));

	Call(Func);
}

void ABuildingContainer::PostUpdate(ABuildingContainer* This, uint8 PersistantState, void* ReservedRandomValues)
{
	PostUpdateOG(This, PersistantState, ReservedRandomValues);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingContainer::PostUpdate: World is null!");
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (FortGameModeAthena) {
		if (UProperty* RedirectAthenaLootTierGroupsProp = FortGameModeAthena->FindPropertyByName("RedirectAthenaLootTierGroups")) {

			uintptr_t RedirectAthenaLootTierGroupsOffset = RedirectAthenaLootTierGroupsProp->Offset_Internal;

			if (Version::Fortnite_Version >= 20)
			{
				auto& RedirectAthenaLootTierGroups = *(TMap<int32, int32>*)(__int64(FortGameModeAthena) + RedirectAthenaLootTierGroupsOffset);

				for (int i = 0; i < RedirectAthenaLootTierGroups.Num(); i++) {
					auto& Pair = RedirectAthenaLootTierGroups[i];

					int32 OldTierGroup = Pair.Key();
					int32 RedirectedTierGroup = Pair.Value();

					if (OldTierGroup == This->SearchLootTierGroup.ComparisonIndex)
					{
						This->SearchLootTierGroup.ComparisonIndex = RedirectedTierGroup;
						break;
					}
				}
			}
			else
			{
				auto& RedirectAthenaLootTierGroups = *(TMap<FName, FName>*)(__int64(FortGameModeAthena) + RedirectAthenaLootTierGroupsOffset);

				for (int i = 0; i < RedirectAthenaLootTierGroups.Num(); i++) {
					auto& Pair = RedirectAthenaLootTierGroups[i];

					FName OldTierGroup = Pair.Key();
					FName RedirectedTierGroup = Pair.Value();

					if (OldTierGroup == This->SearchLootTierGroup)
					{
						This->SearchLootTierGroup = RedirectedTierGroup;
						break;
					}
				}
			}
		}
		else {
			static FName Loot_Treasure = UKismetStringLibrary::Conv_StringToName("Loot_Treasure");
			static FName Loot_Ammo = UKismetStringLibrary::Conv_StringToName("Loot_Ammo");
			static FName Loot_AthenaTreasure = UKismetStringLibrary::Conv_StringToName("Loot_AthenaTreasure");
			static FName Loot_AthenaAmmoLarge = UKismetStringLibrary::Conv_StringToName("Loot_AthenaAmmoLarge");
			static FName Loot_AthenaFloorLoot = UKismetStringLibrary::Conv_StringToName("Loot_AthenaFloorLoot");
			static FName Loot_AthenaFloorLoot_Warmup = UKismetStringLibrary::Conv_StringToName("Loot_AthenaFloorLoot_Warmup");

			if (This->SearchLootTierGroup == Loot_Treasure) {
				This->SearchLootTierGroup = Loot_AthenaTreasure;
				This->bDestroyContainerOnSearch = false;
			}
			else if (This->SearchLootTierGroup == Loot_Ammo) {
				This->SearchLootTierGroup = Loot_AthenaAmmoLarge;
				This->bDestroyContainerOnSearch = false;
			}
			else if (This->SearchLootTierGroup == Loot_AthenaFloorLoot) {
				This->bDestroyContainerOnSearch = false;
			}
			else if (This->SearchLootTierGroup == Loot_AthenaFloorLoot_Warmup) {
				This->bDestroyContainerOnSearch = false;
			}
			else {
				if (Version::Fortnite_Version <= 2.5) {
					This->SearchedMesh = nullptr;
					This->bAllowInteract = false;
					This->bAlreadySearched = true;
					This->OnRep_bAlreadySearched();
				}
			}
		}
	}

	if (This->bStartAlreadySearched_Athena) {
		SpawnLoot(This, nullptr, EFortPickupSourceTypeFlag::GetContainer(), EFortPickupSpawnSource::GetUnset());
	}
}

void ABuildingContainer::OnSetSearched()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnSetSearched");

	Call(Func);
}