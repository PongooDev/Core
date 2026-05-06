#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"

bool ABuildingContainer::SpawnLoot(ABuildingContainer* This, AFortPlayerPawn* PlayerPawn, EFortPickupSourceTypeFlag InSourceTypeFlag, EFortPickupSpawnSource InSpawnSource) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingContainer::SpawnLoot: World is null!");
		return false;
	}

	AFortGameModeAthena* GameMode = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!GameMode) {
		Log("ABuildingContainer::SpawnLoot: GameMode is not AFortGameModeAthena!");
		return false;
	}

	FName TierGroup = This->SearchLootTierGroup;

	if (UProperty* RedirectAthenaLootTierGroupsProp = GameMode->FindPropertyByName("RedirectAthenaLootTierGroups")) {
		
		uintptr_t RedirectAthenaLootTierGroupsOffset = RedirectAthenaLootTierGroupsProp->Offset_Internal;
		
		if (Version::Fortnite_Version >= 20)
		{
			auto& RedirectAthenaLootTierGroups = *(TMap<int32, int32>*)(__int64(GameMode) + RedirectAthenaLootTierGroupsOffset);

			for (int i = 0; i < RedirectAthenaLootTierGroups.Num(); i++) {
				auto& Pair = RedirectAthenaLootTierGroups[i];

				int32 OldTierGroup = Pair.Key();
				int32 RedirectedTierGroup = Pair.Value();

				if (OldTierGroup == This->SearchLootTierGroup.ComparisonIndex)
				{
					TierGroup.ComparisonIndex = RedirectedTierGroup;
					break;
				}
			}
		}
		else
		{
			auto& RedirectAthenaLootTierGroups = *(TMap<FName, FName>*)(__int64(GameMode) + RedirectAthenaLootTierGroupsOffset);

			for (int i = 0; i < RedirectAthenaLootTierGroups.Num(); i++) {
				auto& Pair = RedirectAthenaLootTierGroups[i];

				FName OldTierGroup = Pair.Key();
				FName RedirectedTierGroup = Pair.Value();

				if (OldTierGroup == This->SearchLootTierGroup)
				{
					TierGroup = RedirectedTierGroup;
					break;
				}
			}
		}
	}
	else {
		//Log("ABuildingContainer::SpawnLoot: Failed to find RedirectAthenaLootTierGroups property on GameMode!");
		static FName Loot_Treasure = UKismetStringLibrary::Conv_StringToName("Loot_Treasure");
		static FName Loot_Ammo = UKismetStringLibrary::Conv_StringToName("Loot_Ammo");
		static FName Loot_AthenaTreasure = UKismetStringLibrary::Conv_StringToName("Loot_AthenaTreasure");
		static FName Loot_AthenaAmmoLarge = UKismetStringLibrary::Conv_StringToName("Loot_AthenaAmmoLarge");

		if (This->SearchLootTierGroup == Loot_Treasure)
			TierGroup = Loot_AthenaTreasure;
		else if (This->SearchLootTierGroup == Loot_Ammo)
			TierGroup = Loot_AthenaAmmoLarge;
	}

	FVector ContainerLocation = This->K2_GetActorLocation();
	FVector LootSpawnLocation = This->LootSpawnLocation;
	FVector FinalSpawnLocation = ContainerLocation + (This->GetActorForwardVector() * LootSpawnLocation.X) +
		(This->GetActorRightVector() * LootSpawnLocation.Y) + (This->GetActorUpVector() * LootSpawnLocation.Z);

	TArray<FFortItemEntry> LootDrops;

	bool bSuccess = UFortKismetLibrary::PickLootDrops(This, LootDrops, TierGroup, 0, This->ReplicatedLootTier);

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
			EFortPickupSourceTypeFlag::Container,
			EFortPickupSpawnSource::Unset,
			nullptr,
			false
		);
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