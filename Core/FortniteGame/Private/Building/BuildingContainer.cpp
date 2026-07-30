#include "pch.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"

#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortGlobals.h"

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

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	AFortGameStateZone* GameState = World->GameState->Cast<AFortGameStateZone>();
	if (!GameMode || !GameState) {
		return false;
	}

	FVector ContainerLocation = This->K2_GetActorLocation();
	FVector LootSpawnLocation = This->LootSpawnLocation;
	FVector FinalSpawnLocation = ContainerLocation + (This->GetActorForwardVector() * LootSpawnLocation.X) +
		(This->GetActorRightVector() * LootSpawnLocation.Y) + (This->GetActorUpVector() * LootSpawnLocation.Z);
	FVector LootFinalLocation = ContainerLocation + (This->GetActorForwardVector() * This->LootFinalLocation.X) + (This->GetActorRightVector() * This->LootFinalLocation.Y) +
		(This->GetActorUpVector() * This->LootFinalLocation.Z);

	TArray<FFortItemEntry> LootDrops;
	bool bSuccess = UFortKismetLibrary::PickLootDrops(This, &LootDrops, This->SearchLootTierGroup, GameState->WorldLevel, This->ReplicatedLootTier);

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
		if (GameMode->AssociatedSubGame == ESubGame::GetCampaign()) {
			if (PlayerPawn) {
				PlayerPawn->ServerHandlePickup(PlayerPawn, Pickup, 1.0f, *FVector::Allocate(), true);
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

bool ABuildingContainer::Setup() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingContainer::PostUpdate: World is null!");
		return false;
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

					if (OldTierGroup == SearchLootTierGroup.ComparisonIndex)
					{
						SearchLootTierGroup.ComparisonIndex = RedirectedTierGroup;
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

					if (OldTierGroup == SearchLootTierGroup)
					{
						SearchLootTierGroup = RedirectedTierGroup;
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

			if (SearchLootTierGroup == Loot_Treasure) {
				SearchLootTierGroup = Loot_AthenaTreasure;
				bDestroyContainerOnSearch = false;
			}
			else if (SearchLootTierGroup == Loot_Ammo) {
				SearchLootTierGroup = Loot_AthenaAmmoLarge;
				bDestroyContainerOnSearch = false;
			}
			else if (SearchLootTierGroup == Loot_AthenaFloorLoot) {
				bDestroyContainerOnSearch = false;
			}
			else if (SearchLootTierGroup == Loot_AthenaFloorLoot_Warmup) {
				bDestroyContainerOnSearch = false;
			}
			else {
				if (Version::Fortnite_Version <= 2.5) {
					SearchedMesh = nullptr;
					bAllowInteract = false;
					bAlreadySearched = true;
					OnRep_bAlreadySearched();
				}
			}
		}
	}

	if (bStartAlreadySearched_Athena) {
		SpawnLoot(this, nullptr, EFortPickupSourceTypeFlag::GetContainer(), EFortPickupSpawnSource::GetUnset());
	}
}

void ABuildingContainer::PostUpdate(ABuildingContainer* This, uint8 PersistantState, void* ReservedRandomValues)
{
	PostUpdateOG(This, PersistantState, ReservedRandomValues);
}

void ABuildingContainer::OnSetSearched()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnSetSearched");

	Call(Func);
}
