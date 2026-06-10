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
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortWeapon/FortWeaponStats.h"

class UFortResourceItemDefinition* UFortKismetLibrary::K2_GetResourceItemDefinition(const uint8 ResourceType)
{
	if (Version::Fortnite_Version >= 1.9) {
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

uint8 UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	UClass* ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	uint8* MarkerOptionalAdjustment)
{
	uint8(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, UClass*, FVector, FRotator, bool, TArray<ABuildingActor*>*, uint8*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
	return CanPlaceBuildableClassInStructuralGridInternal(WorldContextObject, ClassToBuild, WorldLocation, WorldRotation, bMirrored, ExistingBuildings, MarkerOptionalAdjustment);
}

uint8 UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(
	UObject* WorldContextObject,
	TSubclassOf<ABuildingSMActor> ClassToBuild,
	FVector WorldLocation,
	FRotator WorldRotation,
	bool bMirrored,
	TArray<ABuildingActor*>* ExistingBuildings,
	uint8* MarkerOptionalAdjustment)
{
	uint8(*CanPlaceBuildableClassInStructuralGridInternal)(UObject*, TSubclassOf<ABuildingSMActor>, FVector, FRotator, bool, TArray<ABuildingActor*>*, uint8*) = decltype(CanPlaceBuildableClassInStructuralGridInternal)(ImageBase + Finder::FindUFortKismetLibrary_CanPlaceBuildableClassInStructuralGrid());
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
	uint8 SourceType,
	uint8 Source,
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

	/*Log(
		"UFortKismetLibrary::K2_SpawnPickupInWorld: Spawning Pickup for Item: "
		+ ItemDefinition->GetName().ToString() +
		" Amount: " + std::to_string(NumberToSpawn) +
		" In World: " + World->GetName().ToString()
	);*/

	AFortPickup* Pickup = World->SpawnActor(AFortPickup::StaticClass(), Position)->Cast<AFortPickup>();
	if (!Pickup) {
		Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Failed to spawn pickup!");
		return nullptr;
	}
	Pickup->bRandomRotation = bRandomRotation;

	FFortItemEntry& PickupEntry = Pickup->PrimaryPickupItemEntry;
	PickupEntry.ItemDefinition = ItemDefinition;
	PickupEntry.SetCount(NumberToSpawn);

	PickupEntry.ReplicationKey++;
	Pickup->OnRep_PrimaryPickupItemEntry();

	Pickup->SetPickupItems(&PickupEntry, &Pickup->MultiItemPickupEntries, SourceType, false, Source);

	AFortPawn* ItemOwner = nullptr;
	if (OptionalOwnerPC) {
		ItemOwner = OptionalOwnerPC->Pawn->Cast<AFortPawn>();
	}
	if (ItemOwner && bBlockedFromAutoPickup) {
		Pickup->PawnWhoDroppedPickup = ItemOwner;
	}

	Pickup->TossPickup(
		Position,
		OptionalOwnerPC ? OptionalOwnerPC->Pawn->Cast<AFortPawn>() : nullptr,
		OverrideMaxStackCount,
		bToss,
		true,
		SourceType,
		Source
	);

	if (SourceType == EFortPickupSourceTypeFlag::GetContainer())
	{
		Pickup->bTossedFromContainer = true;
		Pickup->OnRep_TossedFromContainer();
	}

	if (bPickupOnlyRelevantToOwner) {
		Pickup->bOnlyRelevantToOwner = true;
	}

	//Pickup->ForceNetUpdate();

	//Log("UFortKismetLibrary::K2_SpawnPickupInWorld: Spawned Pickup: " + Pickup->GetName().ToString());
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
	uint8 SourceType = 0;
	uint8 Source = 0;
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
		else if (Name == "ReturnValue") {}
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

	FortPlayerController->WorldInventory->AddItemAndHandleOverflow(ItemDefinition, NumberToGive);

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
	TArray<FFortItemEntry>* OutLootToDrop,
	FName TierGroupName,
	int32 WorldLevel,
	int32 ForcedLootTier)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to get world!");
		return false;
	}

	AFortGameMode* FortGameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!FortGameMode) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to cast AuthorityGameMode to AFortGameMode, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return false;
	}

	AFortGameState* FortGameState = World->GameState->Cast<AFortGameState>();
	if (!FortGameState) {
		Log("UFortKismetLibrary::PickLootDrops: Failed to cast GameState to AFortGameState, GameState: " + World->GameState->GetFullName());
		return false;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();

	/*Log(
		"UFortKismetLibrary::PickLootDrops: Picking loot drops for TierGroup: "
		+ TierGroupName.ToString().ToString() +
		" WorldLevel: " + std::to_string(WorldLevel) +
		" ForcedLootTier: " + std::to_string(ForcedLootTier) +
		" In World: " + World->GetName().ToString()
	);*/

	TArray<UDataTable*> LootTierDataTables;
	TArray<UDataTable*> LootPackagesDataTables;
	if (LootTierDataTables.Num() == 0 || LootPackagesDataTables.Num() == 0) {
		if (FortGameModeAthena && FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist) {
			UDataTable* MainLTD = StaticLoadObject<UDataTable>(
				FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist->LootTierData.ObjectID.AssetPathName.ToString().ToString()
			);

			UDataTable* MainLP = StaticLoadObject<UDataTable>(
				FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist->LootPackages.ObjectID.AssetPathName.ToString().ToString()
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
			UDataTable* DefaultLTD = nullptr;
			if (FortGameModeAthena) {
				DefaultLTD = StaticLoadObject<UDataTable>(
					"/Game/Items/Datatables/AthenaLootTierData_Client.AthenaLootTierData_Client"
				);
			}
			else {
				DefaultLTD = (UDataTable*)StaticLoadObject(
					"/Game/Items/Datatables/LootTierData_Client.LootTierData_Client"
				);
			}
			if (DefaultLTD) {
				LootTierDataTables.Add(DefaultLTD);
			}
			else {
				Log("UFortKismetLibrary::PickLootDrops: Failed to load default loot tier data table!");
			}
		}
		if (LootPackagesDataTables.Num() == 0) {
			UDataTable* DefaultLP = nullptr;
			if (FortGameModeAthena) {
				DefaultLP = StaticLoadObject<UDataTable>(
					"/Game/Items/Datatables/AthenaLootPackages_Client.AthenaLootPackages_Client"
				);
			}
			else {
				DefaultLP = (UDataTable*)StaticLoadObject(
					"/Game/Items/Datatables/LootPackages_Client.LootPackages_Client"
				);
			}

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
		TierGroupName,
		WorldLevel,
		ForcedLootTier
	);

	TArray<FFortItemEntry> LootItems = FFortLootPackageData::GetLootItems(
		LootPackagesDataTables,
		LootTierData,
		WorldLevel
	);

	for (int i = 0; i < LootItems.Num(); i++) {
		FFortItemEntry& LootItem = LootItems.GetWithSize(i, FFortItemEntry::GetSize());
		OutLootToDrop->Add(LootItem, FFortItemEntry::GetSize());
	}

	if (OutLootToDrop->Num() > 0) {
		//Log("UFortKismetLibrary::PickLootDrops: Successfully picked " + std::to_string(OutLootToDrop.Num()) + " loot items to drop!");
		return true;
	}

	//Log("UFortKismetLibrary::PickLootDrops: No loot items were picked to drop!");
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
	TArray<FFortItemEntry>& OutLootToDrop = Stack.StepCompiledInRef<TArray<FFortItemEntry>>();
	FName TierGroupName = FName();
	int32 WorldLevel = -1;
	int32 ForcedLootTier = -1;
	for (auto& Param : PickLootDropsFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
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
		else if (Name == "OutLootToDrop") {
			// Already stepped, do nothing
		}
		else if (Name == "ReturnValue") {}
		else {
			Log("UFortKismetLibrary::execPickLootDrops: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = PickLootDrops(WorldContextObject, &OutLootToDrop, TierGroupName, WorldLevel, ForcedLootTier);
}

AFortAIGoalManager* UFortKismetLibrary::GetAIGoalManager(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::GetAIGoalManager: Failed to get world!");
		return nullptr;
	}

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode) {
		Log("UFortKismetLibrary::GetAIGoalManager: Failed to cast AuthorityGameMode to AFortGameModeZone, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return nullptr;
	}

	Log(
		"UFortKismetLibrary::GetAIGoalManager: Successfully got AIGoalManager: "
		+ GameMode->AIGoalManager->GetName().ToString() +
		" From GameMode: "
		+ GameMode->GetName().ToString()
	);
	return GameMode->AIGoalManager;
}

void UFortKismetLibrary::execGetAIGoalManager(UObject* Object, FFrame& Stack, AFortAIGoalManager** Result)
{
	static UFunction* GetAIGoalManagerFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.GetAIGoalManager");
	if (!GetAIGoalManagerFn) {
		Log("UFortKismetLibrary::execGetAIGoalManager: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	for (auto& Param : GetAIGoalManagerFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
		}
		else {
			Log("UFortKismetLibrary::execGetAIGoalManager: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = GetAIGoalManager(WorldContextObject);
}

void UFortKismetLibrary::K2_GiveItemToAllPlayers(
	UObject* WorldContextObject,
	UFortWorldItemDefinition* ItemDefinition,
	FGuid& ItemVariantGuid,
	int NumberToGive)
{
	if (!ItemDefinition) {
		Log("UFortKismetLibrary::K2_GiveItemToAllPlayers: Failed to get item definition from stack!");
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::K2_GiveItemToAllPlayers: Failed to get world!");
		return;
	}

	AFortGameModeAthena* GameMode = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!GameMode) {
		Log("UFortKismetLibrary::K2_GiveItemToAllPlayers: Failed to cast AuthorityGameMode to AFortGameModeAthena, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return;
	}

	Log(
		"UFortKismetLibrary::K2_GiveItemToAllPlayers: Giving Item: "
		+ ItemDefinition->GetName().ToString() +
		" Amount: " + std::to_string(NumberToGive) +
		" To All Players in World: "
		+ World->GetName().ToString()
	);

	for (int i = 0; i < GameMode->AlivePlayers.Num(); i++) {
		AFortPlayerControllerAthena* PlayerController = GameMode->AlivePlayers[i];
		if (PlayerController) {
			PlayerController->WorldInventory->AddItemAndHandleOverflow(ItemDefinition, NumberToGive);
		}
		else {
			Log("UFortKismetLibrary::K2_GiveItemToAllPlayers: Failed to get player controller for player at index: " + std::to_string(i));
		}
	}
}

void UFortKismetLibrary::execK2_GiveItemToAllPlayers(UObject* Object, FFrame& Stack)
{
	static UFunction* K2_GiveItemToAllPlayersFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.K2_GiveItemToAllPlayers");
	if (!K2_GiveItemToAllPlayersFn) {
		Log("UFortKismetLibrary::execK2_GiveItemToAllPlayers: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	UFortWorldItemDefinition* ItemDefinition = nullptr;
	FGuid ItemVariantGuid = FGuid();
	int32 NumberToGive = 0;
	bool bNotifyPlayer = false;
	for (auto& Param : K2_GiveItemToAllPlayersFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
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
		else {
			Log("UFortKismetLibrary::execK2_GiveItemToAllPlayers: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	K2_GiveItemToAllPlayers(WorldContextObject, ItemDefinition, ItemVariantGuid, NumberToGive);
}

AFortAIDirector* UFortKismetLibrary::GetAIDirector(UObject* WorldContextObject)
{
	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::GetAIDirector: Failed to get world!");
		return nullptr;
	}

	AFortGameModeZone* GameMode = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!GameMode) {
		Log("UFortKismetLibrary::GetAIDirector: Failed to cast AuthorityGameMode to AFortGameModeZone, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return nullptr;
	}

	Log(
		"UFortKismetLibrary::GetAIDirector: Successfully got AIDirector: "
		+ GameMode->AIDirector->GetName().ToString() +
		" From GameMode: "
		+ GameMode->GetName().ToString()
	);

	return GameMode->AIDirector;
}

void UFortKismetLibrary::execGetAIDirector(UObject* Object, FFrame& Stack, AFortAIDirector** Result)
{
	static UFunction* GetAIDirectorFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.GetAIDirector");
	if (!GetAIDirectorFn) {
		Log("UFortKismetLibrary::execGetAIDirector: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	for (auto& Param : GetAIDirectorFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
		}
		else {
			Log("UFortKismetLibrary::execGetAIDirector: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	*Result = GetAIDirector(WorldContextObject);
}

void UFortKismetLibrary::K2_RemoveItemFromAllPlayers(
	UObject* WorldContextObject,
	UFortWorldItemDefinition* ItemDefinition,
	FGuid* ItemVariantGuid,
	int AmountToRemove) {
	if (!ItemDefinition) {
		Log("UFortKismetLibrary::K2_RemoveItemFromAllPlayers: Failed to get item definition from stack!");
		return;
	}

	UWorld* World = WorldContextObject->GetWorld();
	if (!World) {
		Log("UFortKismetLibrary::K2_RemoveItemFromAllPlayers: Failed to get world!");
		return;
	}

	AFortGameModeAthena* GameMode = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!GameMode) {
		Log("UFortKismetLibrary::K2_RemoveItemFromAllPlayers: Failed to cast AuthorityGameMode to AFortGameModeAthena, AuthorityGameMode: " + World->AuthorityGameMode->GetFullName());
		return;
	}

	Log(
		"UFortKismetLibrary::K2_RemoveItemFromAllPlayers: Removing Item: "
		+ ItemDefinition->GetName().ToString() +
		" Amount: " + std::to_string(AmountToRemove) +
		" From All Players in World: "
		+ World->GetName().ToString()
	);

	for (int i = 0; i < GameMode->AlivePlayers.Num(); i++) {
		AFortPlayerControllerAthena* PlayerController = GameMode->AlivePlayers[i];
		if (PlayerController) {
			PlayerController->WorldInventory->RemoveItem(
				ItemDefinition,
				AmountToRemove
			);
		}
		else {
			Log("UFortKismetLibrary::K2_RemoveItemFromAllPlayers: Failed to get player controller for player at index: " + std::to_string(i));
		}
	}
}

void UFortKismetLibrary::execK2_RemoveItemFromAllPlayers(UObject* Object, FFrame& Stack)
{
	static UFunction* K2_RemoveItemFromAllPlayersFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortKismetLibrary.K2_RemoveItemFromAllPlayers");
	if (!K2_RemoveItemFromAllPlayersFn) {
		Log("UFortKismetLibrary::execK2_RemoveItemFromAllPlayers: Failed to find function!");
		return;
	}
	UObject* WorldContextObject = nullptr;
	UFortWorldItemDefinition* ItemDefinition = nullptr;
	FGuid ItemVariantGuid = FGuid();
	int32 AmountToRemove = 0;
	for (auto& Param : K2_RemoveItemFromAllPlayersFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "WorldContextObject") {
			Stack.StepCompiledIn(&WorldContextObject);
		}
		else if (Name == "ItemDefinition") {
			Stack.StepCompiledIn(&ItemDefinition);
		}
		else if (Name == "ItemVariantGuid") {
			Stack.StepCompiledIn(&ItemVariantGuid);
		}
		else if (Name == "AmountToRemove") {
			Stack.StepCompiledIn(&AmountToRemove);
		}
		else {
			Log("UFortKismetLibrary::execK2_RemoveItemFromAllPlayers: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	K2_RemoveItemFromAllPlayers(WorldContextObject, ItemDefinition, &ItemVariantGuid, AmountToRemove);
}

bool UFortKismetLibrary::GetWeaponStatsRow(const FDataTableRowHandle& DataTableRowHandle, FFortBaseWeaponStats* OutRow)
{
	bool (*GetWeaponStatsRowInternal)(const FDataTableRowHandle&, FFortBaseWeaponStats*) = decltype(GetWeaponStatsRowInternal)(ImageBase + Finder::FindUFortKismetLibrary_GetWeaponStatsRow());
	return GetWeaponStatsRowInternal(DataTableRowHandle, OutRow);
}