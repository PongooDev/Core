#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

bool ABuildingContainer::SpawnLoot(ABuildingContainer* This, AFortPlayerPawn* PlayerPawn, uint8 InSourceTypeFlag, uint8 InSpawnSource) {
	if (This->bAlreadySearched) {
		return false;
	}
	
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingContainer::SpawnLoot: World is null!");
		return false;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();

	FVector ContainerLocation = This->K2_GetActorLocation();
	FVector LootSpawnLocation = This->LootSpawnLocation;
	FVector FinalSpawnLocation = ContainerLocation + (This->GetActorForwardVector() * LootSpawnLocation.X) +
		(This->GetActorRightVector() * LootSpawnLocation.Y) + (This->GetActorUpVector() * LootSpawnLocation.Z);

	TArray<FFortItemEntry> LootDrops;

	bool bSuccess = UFortKismetLibrary::PickLootDrops(This, LootDrops, This->SearchLootTierGroup, 0, This->ReplicatedLootTier);

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
			FVector(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetContainer(),
			EFortPickupSpawnSource::GetUnset(),
			nullptr,
			false
		);
		if (Pickup) {
			UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
			if (WeaponDef) {
				int32 Level = Pickup->PrimaryPickupItemEntry.Level;
				Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
				if (FortGameModeAthena) {
					Pickup->PrimaryPickupItemEntry.Durability = FLT_MAX;
				}
				else {
					Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
				}
			}
		}
	}

	This->bAlreadySearched = true;
	This->OnRep_bAlreadySearched();
	This->SearchBounceData.SearchAnimationCount++;
	This->BounceContainer();

	This->ForceNetUpdate();

	if (This->bDestroyContainerOnSearch) {
		This->K2_DestroyActor();
	}

	return bSuccess;
}

void ABuildingContainer::OnRep_bAlreadySearched()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_bAlreadySearched"));

	ProcessEvent(Func, nullptr);
}

void ABuildingContainer::BounceContainer()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"BounceContainer"));

	ProcessEvent(Func, nullptr);
}

void ABuildingContainer::PostUpdate(ABuildingContainer* This)
{
	PostUpdateOG(This);

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
			static auto Loot_AthenaFloorLoot = UKismetStringLibrary::Conv_StringToName(L"Loot_AthenaFloorLoot");
			static auto Loot_AthenaFloorLoot_Warmup = UKismetStringLibrary::Conv_StringToName(L"Loot_AthenaFloorLoot_Warmup");

			if (This->SearchLootTierGroup == Loot_Treasure) {
				This->SearchLootTierGroup = Loot_AthenaTreasure;
				This->bDestroyContainerOnSearch = false;
			}
			else if (This->SearchLootTierGroup == Loot_Ammo) {
				This->SearchLootTierGroup = Loot_AthenaAmmoLarge;
				This->bDestroyContainerOnSearch = false;
			}
			else if (This->SearchLootTierGroup == Loot_AthenaFloorLoot || This->SearchLootTierGroup == Loot_AthenaFloorLoot_Warmup) {
				This->bDestroyContainerOnSearch = true;
			}
		}
	}

	if (This->bStartAlreadySearched_Athena == 1) {
		SpawnLoot(This, nullptr, EFortPickupSourceTypeFlag::GetContainer(), EFortPickupSpawnSource::GetUnset());
	}
}