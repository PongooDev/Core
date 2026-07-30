#include "pch.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"

#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Playlists/FortPlaylistAthena.h"
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

bool ABuildingItemCollectorActor::GrantOutput() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingItemCollectorActor::GrantOutput: World is nullptr!");
		return false;
	}
	
	AFortPlayerController* PC = ControllingPlayer;
	if (!PC) {
		Log("ABuildingItemCollectorActor::GrantOutput: PC is nullptr!");
		return false;
	}

	FColletorUnitInfo* Collection = nullptr;
	for (int32 i = 0; i < ItemCollections.Num(); i++) {
		FColletorUnitInfo& CollectorUnitInfo = ItemCollections.GetWithSize(i, FColletorUnitInfo::GetSize());
		if (CollectorUnitInfo.InputItem == ActiveInputItem) {
			Collection = &CollectorUnitInfo;
			break;
		}
	}

	if (!Collection) {
		Log("ABuildingItemCollectorActor::GrantOutput: Collection not found!");
		return false;
	}

	ClientPausedActiveInputItem = nullptr;

	float Cost = Collection->InputCount.Evaluate();
	float FinalCost = Cost * (StartingGoalLevel + 1);

	UFortItemDefinition* InputItemDef = Collection->InputItem;

	if (FinalCost > 0) {
		FFortItemEntry* ItemEntry = PC->WorldInventory->FindItemEntry(InputItemDef);
		if (ItemEntry && ItemEntry->Count >= FinalCost) {
			PC->WorldInventory->RemoveItem(ItemEntry->ItemGuid, FinalCost);
		}
		else {
			return false;
		}
	}

	FVector ItemCollectorLocation = K2_GetActorLocation();
	FVector FinalSpawnLocation = ItemCollectorLocation + (GetActorForwardVector() * LootSpawnLocation.X) +
		(GetActorRightVector() * LootSpawnLocation.Y) + (GetActorUpVector() * LootSpawnLocation.Z);

	for (int i = 0; i < Collection->OutputItemEntry.Num(); i++) {
		FFortItemEntry& ItemEntry = Collection->OutputItemEntry.GetWithSize(i, FFortItemEntry::GetSize());
		if (!ItemEntry.ItemDefinition) {
			continue;
		}

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry.ItemDefinition,
			ItemEntry.Count,
			FinalSpawnLocation,
			*FVector::Allocate(),
			-1,
			bTossOnGround,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetContainer(),
			EFortPickupSpawnSource::GetVendingMachine(),
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

	if (Cost <= 0) {
		K2_DestroyActor();
	}

	return true;
}

bool ABuildingItemCollectorActor::Setup() {
	UWorld* World = UWorld::GetWorld();
	if (!World || !World->AuthorityGameMode || !World->GameState)
		return false;

	AFortGameMode* GameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	AFortGameState* GameState = World->GameState->Cast<AFortGameState>();
	if (!GameMode || !GameState)
		return false;

	if (!GameMode->bWorldIsReady) {
		return false;
	}

	AFortGameModeAthena* GameModeAthena = GameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* GameStateAthena = GameState->Cast<AFortGameStateAthena>();

	UClass* VendingMachineClass = nullptr;
	if (GameStateAthena && GameStateAthena->MapInfo) {
		VendingMachineClass = GameStateAthena->MapInfo->VendingMachineClass.Class;
	}

	if (VendingMachineClass && IsA(VendingMachineClass))
	{
		if (!GameModeAthena || !GameStateAthena) {
			Log("ABuildingItemCollectorActor::Setup: GameModeAthena or GameStateAthena is null!");
			return false;
		}

		if (!GameStateAthena->MapInfo) {
			Log("ABuildingItemCollectorActor::Setup: MapInfo is null!");
			return false;
		}

		const auto& RarityCurve = GameStateAthena->MapInfo->VendingMachineRarityCount.Curve;
		if (RarityCurve.RowName.IsNone()) {
			Log("ABuildingItemCollectorActor::Setup: RarityCurve.RowName is None!");
			return false;
		}

		UCurveTable* CurveTable = GameStateAthena->GetGameData();
		if (!CurveTable) {
			CurveTable = RarityCurve.CurveTable;
			//Log("ABuildingItemCollectorActor::Setup: Using fallback CurveTable for vending machine!");
		}
		if (!CurveTable) {
			Log("ABuildingItemCollectorActor::Setup: CurveTable is nullptr!");
			return false;
		}

		float Weights[6] = { 0, 0, 0, 0, 0, 0 };
		float TotalWeight = 0.0f;

		for (int32 i = 0; i < 6; ++i)
		{
			float Weight = 0.0f;
			UDataTableFunctionLibrary::EvaluateCurveTableRow(
				CurveTable,
				RarityCurve.RowName,
				(float)i,
				nullptr,
				&Weight,
				FString()
			);

			if (Weight > 0.0f)
			{
				Weights[i] = Weight;
				TotalWeight += Weight;
			}
		}

		if (TotalWeight > 0.0f)
		{
			float RandomNumber = UKismetMathLibrary::RandomFloatInRange(0.0f, TotalWeight);

			for (int32 i = 0; i < 6; ++i)
			{
				if (Weights[i] <= 0.0f)
					continue;

				if (RandomNumber <= Weights[i])
				{
					StartingGoalLevel = i;
					break;
				}

				RandomNumber -= Weights[i];
			}
		}
	}

	// build the items for each collector unit
	for (int32 i = 0; i < ItemCollections.Num(); i++)
	{
		FColletorUnitInfo& CollectorUnitInfo = ItemCollections.GetWithSize(i, FColletorUnitInfo::GetSize());
		if (CollectorUnitInfo.bUseDefinedOutputItem)
			continue;

		if (CollectorUnitInfo.OutputItemEntry.Num() > 0)
		{
			CollectorUnitInfo.OutputItemEntry.Empty();
			CollectorUnitInfo.OutputItem = nullptr;
		}

		TArray<FFortItemEntry> LootDrops;
		const bool bSuccess = UFortKismetLibrary::PickLootDrops(
			World,
			&LootDrops,
			DefaultItemLootTierGroupName,
			GameState->WorldLevel,
			StartingGoalLevel
		);

		if (!bSuccess || LootDrops.Num() <= 0)
			continue;

		for (int32 j = 0; j < LootDrops.Num(); ++j)
		{
			FFortItemEntry& ItemEntry = LootDrops.GetWithSize(j, FFortItemEntry::GetSize());
			if (!ItemEntry.ItemDefinition)
				continue;

			if (!CollectorUnitInfo.OutputItem)
			{
				CollectorUnitInfo.OutputItem = ItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
			}

			CollectorUnitInfo.OutputItemEntry.Add(ItemEntry, FFortItemEntry::GetSize());
		}
	}

	return true;
}

void ABuildingItemCollectorActor::BeginPlay(ABuildingItemCollectorActor* This) {
	BeginPlayOG(This);

	This->Setup();
}

void ABuildingItemCollectorActor::Hook() {
	HookEveryVTableIdx(
		ABuildingItemCollectorActor::StaticClass(),
		Finder::FindAActor_BeginPlayVFT(),
		BeginPlay,
		(LPVOID*)&BeginPlayOG
	);

	Log("ABuildingItemCollectorActor Hooked");
}
