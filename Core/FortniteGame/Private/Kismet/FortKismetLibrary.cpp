#include "pch.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortPlaylist/PlaylistPropertyArray.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"
#include "FortniteGame/Public/FortLoot/FortLootTierData.h"
#include "FortniteGame/Public/FortLoot/FortLootPackageData.h"

class UFortResourceItemDefinition* UFortKismetLibrary::K2_GetResourceItemDefinition(const uint8 ResourceType)
{
	if (Version::Fortnite_Version >= 1.8) {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.K2_GetResourceItemDefinition");

		struct FortKismetLibrary_K2_GetResourceItemDefinition final
		{
		public:
			uint8 ResourceType;
			UFortResourceItemDefinition* ReturnValue;
		};

		FortKismetLibrary_K2_GetResourceItemDefinition Parms{};

		Parms.ResourceType = ResourceType;

		GetDefaultObj()->ProcessEvent(Func, &Parms);

		return Parms.ReturnValue;
	}
	else {
		UObject* ItemDefinition = nullptr;

		switch (ResourceType)
		{
		case 0:
			ItemDefinition = StaticLoadObject("/Game/Items/ResourcePickups/WoodItemData.WoodItemData");
			break;
		case 1:
			ItemDefinition = StaticLoadObject("/Game/Items/ResourcePickups/StoneItemData.StoneItemData");
			break;
		case 2:
			ItemDefinition = StaticLoadObject("/Game/Items/ResourcePickups/MetalItemData.MetalItemData");
			break;
		case 3:
			ItemDefinition = StaticLoadObject("/Game/Items/ResourcePickups/PermaniteItemData.PermaniteItemData");
			break;
		default:
			Log("UFortKismetLibrary::K2_GetResourceItemDefinition: ResourceType " + std::to_string(ResourceType) + " is not valid!");
			break;
		}

		if (!ItemDefinition)
			return nullptr;

		return (UFortResourceItemDefinition*)ItemDefinition;
	}
}

EFortStructuralGridQueryResults UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	UClass* ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment)
{
	EFortStructuralGridQueryResults(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, UClass*, FVector, FRotator, bool, TArray<ABuildingActor*>*, EFortBuildPreviewMarkerOptionalAdjustment*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
	return CanPlaceBuildableClassInStructuralGridInternal(WorldContextObject, ClassToBuild, WorldLocation, WorldRotation, bMirrored, ExistingBuildings, MarkerOptionalAdjustment);
}

EFortStructuralGridQueryResults UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	TSubclassOf<ABuildingSMActor> ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	EFortBuildPreviewMarkerOptionalAdjustment* MarkerOptionalAdjustment)
{
	EFortStructuralGridQueryResults(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, TSubclassOf<ABuildingSMActor>, FVector, FRotator, bool, TArray<ABuildingActor*>*, EFortBuildPreviewMarkerOptionalAdjustment*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
	return CanPlaceBuildableClassInStructuralGridInternal(WorldContextObject, ClassToBuild, WorldLocation, WorldRotation, bMirrored, ExistingBuildings, MarkerOptionalAdjustment);
}

FFortAbilitySetHandle UFortKismetLibrary::EquipFortAbilitySet(TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor, UFortAbilitySet* AbilitySet, UObject* OverrideSourceObject)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.EquipFortAbilitySet");

	if (!Func) {
		Log("UFortKismetLibrary::EquipFortAbilitySet: Failed to find function!");
		return FFortAbilitySetHandle();
	}

	struct FortKismetLibrary_EquipFortAbilitySet
	{
	public:
		TScriptInterface<IAbilitySystemInterface> AbilitySystemInterfaceActor;
		UFortAbilitySet* AbilitySet;
		UObject* OverrideSourceObject;
		FFortAbilitySetHandle ReturnValue;
	};

	FortKismetLibrary_EquipFortAbilitySet Parms{};

	Parms.AbilitySystemInterfaceActor = AbilitySystemInterfaceActor;
	Parms.AbilitySet = AbilitySet;
	Parms.OverrideSourceObject = OverrideSourceObject;

	GetDefaultObj()->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

AFortPickup* UFortKismetLibrary::K2_SpawnPickupInWorld(
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
	bool bPickupOnlyRelevantToOwner) {
	if (!ItemDefinition) {
		Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Failed to get item definition from stack!");
		return nullptr;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Failed to get world!");
		return nullptr;
	}

	Log(
		"UFortKismetLibrary::K2_SpawnPickupInWorld: Spawning Pickup for Item: "
		+ ItemDefinition->GetName().ToString() +
		" Amount: " + std::to_string(NumberToSpawn) +
		" In World: " + World->GetName().ToString()
	);

	AFortPickup* Pickup = World->SpawnActor(AFortPickup::StaticClass(), Position)->Cast<AFortPickup>();
	if (!Pickup) {
		Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Failed to spawn pickup!");
		return nullptr;
	}
	Pickup->bRandomRotation = bRandomRotation;

	FFortItemEntry& PickupEntry = Pickup->PrimaryPickupItemEntry;
	PickupEntry.ItemDefinition = ItemDefinition;
	PickupEntry.Count = NumberToSpawn;
	PickupEntry.LoadedAmmo = ItemDefinition->GetClipSize();
	PickupEntry.ReplicationKey++;
	Pickup->OnRep_PrimaryPickupItemEntry();

	Pickup->SetPickupItems(&PickupEntry, &Pickup->MultiItemPickupEntries, SourceType, false, Source);

	AFortPawn* ItemOwner = nullptr;
	if (OptionalOwnerPC) {
		ItemOwner = OptionalOwnerPC->Pawn->Cast<AFortPawn>();
	}
	if (ItemOwner) {
		Pickup->PawnWhoDroppedPickup = ItemOwner;
	}

	Pickup->TossPickup(
		Position,
		OptionalOwnerPC ? OptionalOwnerPC->Pawn->Cast<AFortPawn>() : nullptr,
		OverrideMaxStackCount,
		bToss,
		false,
		SourceType,
		Source
	);

	if (SourceType == EFortPickupSourceTypeFlag::Container)
	{
		Pickup->bTossedFromContainer = true;
		Pickup->OnRep_TossedFromContainer();
	}

	if (bPickupOnlyRelevantToOwner) {
		Pickup->bOnlyRelevantToOwner = true;
	}

	Pickup->ForceNetUpdate();

	Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Spawned Pickup: " + Pickup->GetName().ToString());
	return Pickup;
}

void UFortKismetLibrary::execK2_SpawnPickupInWorld(UObject* Object, FFrame& Stack, AFortPickup** Result)
{
	static UFunction* K2_SpawnPickupInWorldFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.K2_SpawnPickupInWorld");
	if (!K2_SpawnPickupInWorldFn) {
		Log("UFortKismetLibrary::execK2_SpawnPickupInWorld: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	UFortWorldItemDefinition* ItemDefinition = nullptr;
	int32 NumberToSpawn = 0;
	FVector Position = FVector();
	FVector Direction = FVector();
	int32 OverrideMaxStackCount = 0;
	bool bToss = false;
	bool bRandomRotation = false;
	bool bBlockedFromAutoPickup = false;
	int32 PickupInstigatorHandle = 0;
	EFortPickupSourceTypeFlag SourceType = EFortPickupSourceTypeFlag();
	EFortPickupSpawnSource Source = EFortPickupSpawnSource();
	AFortPlayerController* OptionalOwnerPC = nullptr;
	bool bPickupOnlyRelevantToOwner = false;
	for (auto& Param : K2_SpawnPickupInWorldFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
		}
		else if (Name == "ItemDefinition") {
			Stack.StepCompiledIn(&ItemDefinition);
		}
		else if (Name == "NumberToSpawn") {
			Stack.StepCompiledIn(&NumberToSpawn);
		}
		else if (Name == "Position") {
			Stack.StepCompiledIn(&Position);
		}
		else if (Name == "Direction") {
			Stack.StepCompiledIn(&Direction);
		}
		else if (Name == "OverrideMaxStackCount") {
			Stack.StepCompiledIn(&OverrideMaxStackCount);
		}
		else if (Name == "bToss") {
			Stack.StepCompiledIn(&bToss);
		}
		else if (Name == "bRandomRotation") {
			Stack.StepCompiledIn(&bRandomRotation);
		}
		else if (Name == "bBlockedFromAutoPickup") {
			Stack.StepCompiledIn(&bBlockedFromAutoPickup);
		}
		else if (Name == "PickupInstigatorHandle") {
			Stack.StepCompiledIn(&PickupInstigatorHandle);
		}
		else if (Name == "SourceType") {
			Stack.StepCompiledIn(&SourceType);
		}
		else if (Name == "Source") {
			Stack.StepCompiledIn(&Source);
		}
		else if (Name == "OptionalOwnerPC") {
			Stack.StepCompiledIn(&OptionalOwnerPC);
		}
		else if (Name == "bPickupOnlyRelevantToOwner") {
			Stack.StepCompiledIn(&bPickupOnlyRelevantToOwner);
		}
		else {
			Log("UFortKismetLibrary::execK2_SpawnPickupInWorld: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = K2_SpawnPickupInWorld(WorldContextObject, ItemDefinition, NumberToSpawn, Position, Direction, OverrideMaxStackCount, bToss, bRandomRotation, bBlockedFromAutoPickup, PickupInstigatorHandle, SourceType, Source, OptionalOwnerPC, bPickupOnlyRelevantToOwner);
}

UFortWorldItem* UFortKismetLibrary::GiveItemToInventoryOwner(
	TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner,
	UFortWorldItemDefinition* ItemDefinition,
	FGuid& ItemVariantGuid,
	int NumberToGive)
{
	if (!ItemDefinition) {
		Log("UFortKismetLibrary::GiveItemToInventoryOwner: Failed to get item definition from stack!");
		return nullptr;
	}

	AFortPlayerController* FortPlayerController = (AFortPlayerController*)InventoryOwner.ObjectPointer;
	if (!FortPlayerController) {
		Log("UFortKismetLibrary::GiveItemToInventoryOwner: Failed to get player controller from inventory owner!");
		return nullptr;
	}

	Log(
		"UFortKismetLibrary::GiveItemToInventoryOwner: Giving Item: "
		+ ItemDefinition->GetName().ToString() +
		" To PlayerController: "
		+ FortPlayerController->GetName().ToString()
	);

	int32 Overflow = FortPlayerController->WorldInventory->GetOverflowFromAddingItem(ItemDefinition, NumberToGive);
	if (Overflow > 0) {
		UFortKismetLibrary::K2_SpawnPickupInWorld(
			FortPlayerController->GetWorld(),
			ItemDefinition,
			Overflow,
			FortPlayerController->Pawn->K2_GetActorLocation(),
			FVector(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::Player,
			EFortPickupSpawnSource::Unset,
			FortPlayerController,
			false
		);
	}

	return FortPlayerController->WorldInventory->FindItemInstance(ItemDefinition);
}

void UFortKismetLibrary::execGiveItemToInventoryOwner(UObject* Object, FFrame& Stack, UFortWorldItem** Result)
{
	static UFunction* GiveItemToInventoryOwnerFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.GiveItemToInventoryOwner");
	if (!GiveItemToInventoryOwnerFn) {
		Log("UFortKismetLibrary::execGiveItemToInventoryOwner: Failed to find function!");
		return;
	}

	TScriptInterface<IFortInventoryOwnerInterface> InventoryOwner;
	UFortWorldItemDefinition* ItemDefinition = nullptr;
	FGuid ItemVariantGuid = FGuid();
	int32 NumberToGive = 0;
	bool bNotifyPlayer = false;
	int32 ItemLevel = 0;
	int32 PickupInstigatorHandle = 0;
	for (auto& Param : GiveItemToInventoryOwnerFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "InventoryOwner") {
			Stack.StepCompiledIn(&InventoryOwner);
		}
		else if (Name == "ItemDefinition") {
			Stack.StepCompiledIn(&ItemDefinition);
		}
		else if (Name == "ItemVariantGuid") {
			Stack.StepCompiledIn(&ItemVariantGuid);
		}
		else if (Name == "NumberToGive") {
			Stack.StepCompiledIn(&NumberToGive);
		}
		else if (Name == "bNotifyPlayer") {
			Stack.StepCompiledIn(&bNotifyPlayer);
		}
		else if (Name == "ItemLevel") {
			Stack.StepCompiledIn(&ItemLevel);
		}
		else if (Name == "PickupInstigatorHandle") {
			Stack.StepCompiledIn(&PickupInstigatorHandle);
		}
		else {
			Log("UFortKismetLibrary::execGiveItemToInventoryOwner: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = GiveItemToInventoryOwner(InventoryOwner, ItemDefinition, ItemVariantGuid, NumberToGive);
}

bool UFortKismetLibrary::PickLootDrops(
	UObject* WorldContextObject,
	TArray<FFortItemEntry>& OutLootToDrop,
	FName TierGroupName,
	int32 WorldLevel,
	int32 ForcedLootTier)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to get world!");
		return false;
	}
	AFortGameModeAthena* GameMode = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!GameMode) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to cast AuthorityGameMode to AFortGameModeAthena, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return false;
	}
	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to cast GameState to AFortGameStateAthena, GameState: " + World->GameState->GetFullName());
		return false;
	}

	Log(
		"UFortKismetLibrary::PickLootDrops: Picking loot drops for TierGroup: "
		+ TierGroupName.ToString().ToString() +
		" WorldLevel: " + std::to_string(WorldLevel) +
		" ForcedLootTier: " + std::to_string(ForcedLootTier) +
		" In World: " + World->GetName().ToString()
	);

	TArray<UDataTable*> LootTierDataTables;
	TArray<UDataTable*> LootPackagesDataTables;
	if (LootTierDataTables.Num() == 0 || LootPackagesDataTables.Num() == 0) {
		if (GameState->CurrentPlaylistInfo.BasePlaylist) {
			UDataTable* MainLTD = StaticLoadObject<UDataTable>(
				GameState->CurrentPlaylistInfo.BasePlaylist->LootTierData.ObjectID.AssetPathName.ToString().ToString()
			);

			UDataTable* MainLP = StaticLoadObject<UDataTable>(
				GameState->CurrentPlaylistInfo.BasePlaylist->LootPackages.ObjectID.AssetPathName.ToString().ToString()
			);

			if (MainLTD) {
				LootTierDataTables.Add(MainLTD);
			}
			else {
				Log("UFortKismetLibrary::PickLootDrops: Failed to load main loot tier data table from playlist!");
			}

			if (MainLP) {
				LootPackagesDataTables.Add(MainLP);
			}
			else {
				Log("UFortKismetLibrary::PickLootDrops: Failed to load main loot packages data table from playlist!");
			}
		}
		if (LootTierDataTables.Num() == 0) {
			UDataTable* DefaultLTD = StaticLoadObject<UDataTable>(
				"/Game/Items/Datatables/AthenaLootTierData_Client.AthenaLootTierData_Client"
			);
			if (DefaultLTD) {
				LootTierDataTables.Add(DefaultLTD);
			}
			else {
				Log("UFortKismetLibrary::PickLootDrops: Failed to load default loot tier data table!");
			}
		}
		if (LootPackagesDataTables.Num() == 0) {
			UDataTable* DefaultLP = StaticLoadObject<UDataTable>(
				"/Game/Items/Datatables/AthenaLootPackages_Client.AthenaLootPackages_Client"
			);

			if (DefaultLP) {
				LootPackagesDataTables.Add(DefaultLP);
			}
			else {
				Log("UFortKismetLibrary::PickLootDrops: Failed to load default loot packages data table!");
			}
		}
	}

	FFortLootTierData* LootTierData = FFortLootTierData::PickLootTierData(
		LootTierDataTables,
		TierGroupName
	);

	TArray<FFortItemEntry> LootItems = FFortLootPackageData::GetLootItems(
		LootPackagesDataTables,
		LootTierData
	);

	for (int i = 0; i < LootItems.Num(); i++) {
		FFortItemEntry& LootItem = LootItems.GetWithSize(i, FFortItemEntry::GetSize());
		OutLootToDrop.Add(LootItem, FFortItemEntry::GetSize());
	}

	if (OutLootToDrop.Num() > 0) {
		Log("UFortKismetLibrary::PickLootDrops: Successfully picked " + std::to_string(OutLootToDrop.Num()) + " loot items to drop!");
		return true;
	}

	Log("UFortKismetLibrary::PickLootDrops: No loot items were picked to drop!");
	return false;
}

void UFortKismetLibrary::execPickLootDrops(UObject* Object, FFrame& Stack, bool* Result)
{
	static UFunction* PickLootDropsFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.PickLootDrops");
	if (!PickLootDropsFn) {
		Log("UFortKismetLibrary::execPickLootDrops: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	TArray<FFortItemEntry> OutLootToDrop;
	FName TierGroupName = FName();
	int32 WorldLevel = 0;
	int32 ForcedLootTier = -1;
	for (auto& Param : PickLootDropsFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
		}
		else if (Name == "OutLootToDrop") {
			OutLootToDrop = Stack.StepCompiledInRef<TArray<FFortItemEntry>>();
		}
		else if (Name == "TierGroupName") {
			Stack.StepCompiledIn(&TierGroupName);
		}
		else if (Name == "WorldLevel") {
			Stack.StepCompiledIn(&WorldLevel);
		}
		else if (Name == "ForcedLootTier") {
			Stack.StepCompiledIn(&ForcedLootTier);
		}
		else if (Name == "ReturnValue") {}
		else {
			Log("UFortKismetLibrary::execPickLootDrops: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = PickLootDrops(WorldContextObject, OutLootToDrop, TierGroupName, WorldLevel, ForcedLootTier);
}