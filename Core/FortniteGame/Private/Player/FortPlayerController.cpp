#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerController.h"

#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Inventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortCheatManager.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Weapons/FortWeapon.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/Events.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Player/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/Heroes/FortHeroType.h"
#include "FortniteGame/Public/Heroes/FortHero.h"
#include "FortniteGame/Public/Customization/CustomCharacterPart.h"
#include "FortniteGame/Public/Heroes/FortHeroSpecialization.h"
#include "FortniteGame/Public/Building/BuildingSMActor.h"
#include "FortniteGame/Public/Building/BuildingContainer.h"
#include "FortniteGame/Public/Building/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Abilities/FortGameplayAbility.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/Athena/FortAthenaAircraft.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Heist.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraft.h"
#include "FortniteGame/Public/Athena/FortSafeZoneIndicator.h"
#include "FortniteGame/Public/Items/Definitions/AthenaDanceItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/AthenaSprayItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/AthenaToyItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortMontageItemDefinitionBase.h"
#include "FortniteGame/Public/Items/Definitions/FortPersonalVehicleItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortResourceItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortEditToolItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortWeap_EditingTool.h"
#include "FortniteGame/Public/FortGlobals.h"

void AFortPlayerController::ClientForceProfileQuery()
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceProfileQuery");

	return Call(Func);
}

void AFortPlayerController::OnReadyToStartMatch(AFortPlayerController* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::OnReadyToStartMatch: World is null!");
		return;
	}

	if (This->_HasQuickBars() && !This->QuickBars)
	{
		This->SpawnQuickBars();
		This->SetupQuickBars();
	}
}

void AFortPlayerController::SpawnQuickBars()
{
	if (Finder::FindAFortPlayerController_SpawnQuickBarsVFT() != 0) {
		void (*&SpawnQuickBarsInternal)(AFortPlayerController* This) = decltype(SpawnQuickBarsInternal)(VTable[Finder::FindAFortPlayerController_SpawnQuickBarsVFT()]);
		SpawnQuickBarsInternal(this);
	}
	else {
		UWorld* World = UWorld::GetWorld();
		if (!World) {
			Log("AFortPlayerController::SpawnQuickBars: World is null!");
			return;
		}

		UClass* QuickBarsClass = AFortQuickBars::GetDefaultQuickBarsClass();

		if (IsUsingOldQuickBars()) {
			if (!QuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(QuickBarsClass, FVector(-280, 400, 3000), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
					OnRep_QuickBar();

					Log("Spawned QuickBars: " + QuickBars->GetName().ToString());
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(QuickBarsClass, FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					ClientQuickBars = NewQuickBars->Cast<AFortQuickBars>();

					Log("Spawned ClientQuickBars: " + ClientQuickBars->GetName().ToString());
				}
			}
		}
	}
}

void AFortPlayerController::SetupQuickBars()
{
	if (Finder::FindAFortPlayerController_SetupQuickBarsVFT()) {
		void (*&SetupQuickBarsInternal)(AFortPlayerController* This) = decltype(SetupQuickBarsInternal)(VTable[Finder::FindAFortPlayerController_SetupQuickBarsVFT()]);
		SetupQuickBarsInternal(this);
	}
}

void AFortPlayerController::ServerCheat(AFortPlayerController* This, FString& Msg) {
	std::string Command = Msg.ToString();
	Log("ServerCheat (" + This->GetName().ToString() + "): [" + Command + "]");

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bIsProd)
		return;
	
	UFortCheatManager* CheatManager = This->CheatManager->Cast<UFortCheatManager>();
	if (!CheatManager)
	{
		UFortCheatManager* NewCheatManager = (UFortCheatManager*)UGameplayStatics::SpawnObject(This->CheatClass.Get(), This);
		if (NewCheatManager) {
			This->CheatManager = NewCheatManager;
			CheatManager = NewCheatManager;
			This->ClientMessage("Spawned CheatManager: " + CheatManager->GetName().ToString());
		}
		else {
			This->ClientMessage("Failed to spawn CheatManager!");
			return;
		}
	}

	ServerCheatOG(This, Msg);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		This->ClientMessage("World is null!");
		return;
	}

	AFortGameMode* GameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!GameMode) {
		This->ClientMessage("GameMode is null or not a FortGameMode!");
		return;
	}

	FCommandParser Parser(Command);

	if (Parser.IsCommand("Help")) {
		CheatManager->Help(Parser);
		return;
	}
	else if (Parser.IsCommand("GiveItem")) {
		CheatManager->GiveItem(Parser);
		return;
	}
	else if (Parser.IsCommand("ForceGiveItem")) {
		CheatManager->ForceGiveItem(Parser);
		return;
	}
	else if (Parser.IsCommand("SpawnPickup")) {
		CheatManager->SpawnPickup(Parser);
		return;
	}
	else if (Parser.IsCommand("SetLoadedAmmo")) {
		CheatManager->SetLoadedAmmo(Parser);
		return;
	}
	else if (Parser.IsCommand("GiveAmmo")) {
		CheatManager->GiveAmmo(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpInventory")) {
		CheatManager->DumpInventory(Parser);
		return;
	}
	else if (Parser.IsCommand("SetHealth")) {
		CheatManager->SetHealth(Parser);
		return;
	}
	else if (Parser.IsCommand("SetShield")) {
		CheatManager->SetShield(Parser);
		return;
	}
	else if (Parser.IsCommand("SetMaxHealth")) {
		CheatManager->SetMaxHealth(Parser);
		return;
	}
	else if (Parser.IsCommand("SetMaxShield")) {
		CheatManager->SetMaxShield(Parser);
		return;
	}
	else if (Parser.IsCommand("SpawnActor")) {
		CheatManager->SpawnActor(Parser);
		return;
	}
	else if (Parser.IsCommand("ClearEquippedItem")) {
		CheatManager->ClearEquippedItem(Parser);
		return;
	}
	else if (Parser.IsCommand("GetWeaponStats")) {
		CheatManager->GetWeaponStats(Parser);
		return;
	}
	else if (Parser.IsCommand("DestroyTarget")) {
		CheatManager->DestroyTarget(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpActorsWithClass")) {
		CheatManager->DumpActorsWithClass(Parser);
		return;
	}
	else if (Parser.IsCommand("TeleportToLocation")) {
		CheatManager->TeleportToLocation(Parser);
		return;
	}
	else if (Parser.IsCommand("TeleportToActor")) {
		CheatManager->TeleportToActor(Parser);
		return;
	}
	else if (Parser.IsCommand("StartEvent")) {
		CheatManager->StartEvent(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpCurrentLocation")) {
		CheatManager->DumpCurrentLocation(Parser);
		return;
	}
	else if (Parser.IsCommand("SpawnQuickBars")) {
		CheatManager->SpawnQuickBars(Parser);
		return;
	}
	else if (Parser.IsCommand("DestroyQuickBars")) {
		CheatManager->DestroyQuickBars(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpQuickBars")) {
		CheatManager->DumpQuickBars(Parser);
		return;
	}
	else if (Parser.IsCommand("ServerExecuteInventoryItem")) {
		CheatManager->ServerExecuteInventoryItem(Parser);
		return;
	}
	else if (Parser.IsCommand("PossessPawnByIndex")) {
		CheatManager->PossessPawnByIndex(Parser);
		return;
	}
	else if (Parser.IsCommand("PossessPawnByName")) {
		CheatManager->PossessPawnByName(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpAllPawns")) {
		CheatManager->DumpAllPawns(Parser);
		return;
	}
	else if (Parser.IsCommand("SetKillScore")) {
		CheatManager->SetKillScore(Parser);
		return;
	}
	else if (Parser.IsCommand("LootRain")) {
		CheatManager->LootRain(Parser);
		return;
	}
	else if (Parser.IsCommand("SetGameSpeed")) {
		CheatManager->SetGameSpeed(Parser);
		return;
	}
	else if (Parser.IsCommand("DespawnAllBots")) {
		CheatManager->DespawnAllBots(Parser);
		return;
	}
	else if (Parser.IsCommand("TeleportAllToMe")) {
		CheatManager->TeleportAllToMe(Parser);
		return;
	}
	else if (Parser.IsCommand("SwapPlaces")) {
		CheatManager->SwapPlaces(Parser);
		return;
	}
	else if (Parser.IsCommand("LaunchPawn")) {
		CheatManager->LaunchPawn(Parser);
		return;
	}
	else if (Parser.IsCommand("SetScale")) {
		CheatManager->SetScale(Parser);
		return;
	}
	else if (Parser.IsCommand("ScalePawn")) {
		CheatManager->ScalePawn(Parser);
		return;
	}
	else if (Parser.IsCommand("Goto")) {
		CheatManager->Goto(Parser);
		return;
	}
	else if (Parser.IsCommand("DestroyBuildings")) {
		CheatManager->DestroyBuildings(Parser);
		return;
	}
	else if (Parser.IsCommand("EmoteAll")) {
		CheatManager->EmoteAll(Parser);
		return;
	}
	else if (Parser.IsCommand("EmoteAllSpecific")) {
		CheatManager->EmoteAllSpecific(Parser);
		return;
	}
	else if (Parser.IsCommand("EmotePlayerByName")) {
		CheatManager->EmotePlayerByName(Parser);
		return;
	}
	else if (Parser.IsCommand("TogglePersonalVehicle")) {
		CheatManager->TogglePersonalVehicle(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpAircrafts")) {
		CheatManager->DumpAircrafts(Parser);
		return;
	}
	else if (Parser.IsCommand("ActivateRifts")) {
		CheatManager->ActivateRifts(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpEncounters")) {
		CheatManager->DumpEncounters(Parser);
		return;
	}
	else if (Parser.IsCommand("StartEncounter")) {
		CheatManager->StartEncounter(Parser);
		return;
	}
	else if (Parser.IsCommand("SetSpawnExitCraftTime")) {
		CheatManager->SetSpawnExitCraftTime(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpExitCraftTimer") || Parser.IsCommand("HeistPhaseStep")) {
		CheatManager->DumpExitCraftTimer(Parser);
		return;
	}
	else if (Parser.IsCommand("UpdateGamePhaseStep")) {
		CheatManager->UpdateGamePhaseStep(Parser);
		return;
	}
	else if (Parser.IsCommand("DumpGameState")) {
		CheatManager->DumpGameState(Parser);
		return;
	}
	else if (Parser.IsCommand("SpawnExitCraft")) {
		CheatManager->SpawnExitCraft(Parser);
		return;
	}
	else if (Parser.IsCommand("ListQuests")) {
		CheatManager->ListQuests(Parser);
		return;
	}
	else if (Parser.IsCommand("ProgressQuest")) {
		CheatManager->ProgressQuest(Parser);
		return;
	}

	if (Version::Fortnite_Version >= 2.2) {
		UKismetSystemLibrary::ExecuteConsoleCommand(UWorld::GetWorld(), Msg, This);
	}
}

void AFortPlayerController::ServerExecuteInventoryItem(AFortPlayerController* This, FGuid& ItemGuid) {
	UFortWorldItem* ItemInstance = This->FindItemInstance(ItemGuid);
	if (!ItemInstance) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemInstance not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	UFortWorldItemDefinition* ItemDef = ItemInstance->ItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
	if (!ItemDef) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: ItemDefinition is not a UFortWorldItemDefinition for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPawn* FortPawn = This->MyFortPawn;
	if (!FortPawn) {
		Log("AFortPlayerController::ServerExecuteInventoryItem: MyFortPawn is null!");
		return;
	}

	//Log("ServerExecuteInventoryItem: " + ItemDef->GetName().ToString() + " (GUID: " + ItemGuid.FormatGuid() + ")");
	ItemDef->ServerExecute(ItemInstance, This);
}

FFortItemEntry* AFortPlayerController::FindItemEntry(FGuid Guid) {
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemEntry: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemEntry(Guid);
}

FFortItemEntry* AFortPlayerController::FindItemEntry(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition) {
		Log("AFortPlayerController::FindItemEntry: ItemDefinition is null!");
		return nullptr;
	}
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemEntry: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemEntry(ItemDefinition);
}

UFortWorldItem* AFortPlayerController::FindItemInstance(FGuid Guid)
{
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemInstance: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemInstance(Guid);
}

UFortWorldItem* AFortPlayerController::FindItemInstance(UFortItemDefinition* ItemDefinition)
{
	if (!ItemDefinition) {
		Log("AFortPlayerController::FindItemInstance: ItemDefinition is null!");
		return nullptr;
	}
	if (!WorldInventory) {
		Log("AFortPlayerController::FindItemInstance: WorldInventory is null!");
		return nullptr;
	}

	return WorldInventory->FindItemInstance(ItemDefinition);
}

void AFortPlayerController::ClientReportDamagedResourceBuilding(ABuildingSMActor* BuildingSMActor, uint8 PotentialResourceType, int32 PotentialResourceCount, bool bDestroyed, bool bJustHitWeakspot)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportDamagedResourceBuilding");

	return Call(Func, BuildingSMActor, PotentialResourceType, PotentialResourceCount, bDestroyed, bJustHitWeakspot);
}

void AFortPlayerController::ServerAttemptInventoryDrop(FGuid& ItemGuid, int Count, bool bTrash) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: World is null!");
		return;
	}

	if (!MyFortPawn) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: MyFortPawn is null!");
		return;
	}

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerAttemptInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count) + ", bTrash: " + std::to_string(bTrash));
	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPickup* Pickup = nullptr;
	if (!bTrash) {
		Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry->ItemDefinition,
			Count,
			FinalLoc,
			GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetTossedByPlayer(),
			this,
			false
		);
		if (Pickup) {
			Pickup->PrimaryPickupItemEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
			Pickup->PrimaryPickupItemEntry.Durability = ItemEntry->Durability;
			Pickup->PrimaryPickupItemEntry.bIsDirty = true;

			Pickup->PrimaryPickupItemEntry.ReplicationKey++;
			Pickup->OnRep_PrimaryPickupItemEntry();
		}
	}
	
	if (bTrash || Pickup) {
		WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
	}
}

void AFortPlayerController::execServerAttemptInventoryDrop(AFortPlayerController* Context, FFrame& Stack) {
	static UFunction* ServerAttemptInventoryDropFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerAttemptInventoryDrop");
	if (!ServerAttemptInventoryDropFn) {
		Log("AFortPlayerController::execServerAttemptInventoryDrop: Failed to find function!");
		return;
	}
	
	FGuid& ItemGuid = Stack.StepCompiledInRef<FGuid>();
	int32 Count = 0;
	bool bTrash = false;
	Stack.StepCompiledIn(&Count);
	
	for (auto& Param : ServerAttemptInventoryDropFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "bTrash") {
			Stack.StepCompiledIn(&bTrash);
		}
	}
	Stack.IncrementCode();

	Context->ServerAttemptInventoryDrop(ItemGuid, Count, bTrash);
}

void AFortPlayerController::ServerSpawnInventoryDrop(FGuid& ItemGuid, int32 Count) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: World is null!");
		return;
	}

	if (!MyFortPawn) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: MyFortPawn is null!");
		return;
	}

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerSpawnInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count));
	FFortItemEntry* ItemEntry = FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("AFortPlayerController::ServerSpawnInventoryDrop: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		ItemEntry->ItemDefinition,
		Count,
		FinalLoc,
		GetDropFinalLocation(),
		-1,
		true,
		true,
		true,
		-1,
		EFortPickupSourceTypeFlag::GetPlayer(),
		EFortPickupSpawnSource::GetTossedByPlayer(),
		this,
		false
	);
	if (Pickup) {
		Pickup->PrimaryPickupItemEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
		Pickup->PrimaryPickupItemEntry.Durability = ItemEntry->Durability;
		Pickup->PrimaryPickupItemEntry.bIsDirty = true;

		Pickup->PrimaryPickupItemEntry.ReplicationKey++;
		Pickup->OnRep_PrimaryPickupItemEntry();

		WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
	}
}

void AFortPlayerController::execServerSpawnInventoryDrop(AFortPlayerController* Context, FFrame& Stack) {
	FGuid& ItemGuid = Stack.StepCompiledInRef<FGuid>();
	int32 Count = 0;
	Stack.StepCompiledIn(&Count);
	Stack.IncrementCode();

	Context->ServerSpawnInventoryDrop(ItemGuid, Count);
}

void AFortPlayerController::ClientForceUpdateQuickbar(uint8 QuickbarToRefresh)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceUpdateQuickbar");

	return Call(Func, QuickbarToRefresh);
}

void AFortPlayerController::OnRep_QuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_QuickBar");

	Call(Func);
}

bool AFortPlayerController::IsUsingOldQuickBars() {
	static uintptr_t ClientQuickBarsOffset = StaticClass()->GetPropertyOffset("ClientQuickBars");

	return ClientQuickBarsOffset == -1;
}

void AFortPlayerController::ServerClientPawnLoaded(AFortPlayerController* This, bool bIsPawnLoaded)
{
	ServerClientPawnLoadedOG(This, bIsPawnLoaded);
	AFortPlayerState* FortPlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerState>() : nullptr;
	if (!FortPlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerPawn* MyFortPawn = This->MyFortPawn;

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();
	AFortPlayerStateAthena* FortPSAthena = FortPlayerState->Cast<AFortPlayerStateAthena>();

	if (bIsPawnLoaded) {
		FortPlayerState->ApplyCharacterCustomization(MyFortPawn);

		if (FortPSAthena) {
			FortPSAthena->SetKillScore(FortPSAthena->KillScore);
		}
	}
}

bool AFortPlayerController::RemoveInventoryItem(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoveFromQuickBars, bool bForceRemoval)
{
	static auto InterfaceOffset = StaticClass()->SuperStruct->PropertiesSize + (Version::Engine_Version >= 4.27 ? 16 : 8);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(__int64(This) - InterfaceOffset); // this is so wierd
	if (!PlayerController) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	AFortInventory* Inventory = PlayerController->WorldInventory;
	if (!Inventory) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	if (!ItemGuid.IsValid()) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	FFortItemEntry* ItemEntry = Inventory->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoveFromQuickBars, bForceRemoval);
	}

	UFortWorldItemDefinition* WorldItemDefinition = ItemEntry->ItemDefinition->Cast<UFortWorldItemDefinition>();

	bool bForceRemoveItem = bForceRemoveFromQuickBars || bForceRemoval;
	if (!bForceRemoveItem) {
		bool bShouldRemoveCount = ItemEntry->Count > Count;
		if (WorldItemDefinition && WorldItemDefinition->bPersistInInventoryWhenFinalStackEmpty) {
			bShouldRemoveCount = ItemEntry->Count >= Count;
		}

		if (bShouldRemoveCount) {
			ItemEntry->Count -= Count;
			return Inventory->Update(ItemEntry);
		}
		else {
			return Inventory->RemoveItem(ItemGuid, Count);
		}
	}
	else {
		return Inventory->RemoveItem(ItemGuid, Count);
	}
}

void AFortPlayerController::ServerCreateBuildingActorOld(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored, float SyncKey) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerCreateBuildingActor: World is null!");
		return;
	}

	UFortResourceItemDefinition* Resource = UFortKismetLibrary::K2_GetResourceItemDefinition(((ABuildingSMActor*)BuildingClassData.BuildingClass->GetDefaultObj())->ResourceType);
	if (!Resource) {
		Log("ServerCreateBuildingActor: Failed to get resource item definition for building class!");
		return;
	}

	if (!This->CanAffordToPlaceBuildableClass(&BuildingClassData)) {
		return;
	}

	TArray<ABuildingActor*> BuildingsToRemove;
	uint8 OptionalAdjustment;
	if (UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(World, BuildingClassData.BuildingClass.Get(), BuildLoc, BuildRot, bMirrored, &BuildingsToRemove, &OptionalAdjustment)) {
		return;
	}

	for (ABuildingActor* BuildingToRemove : BuildingsToRemove) {
		if (BuildingToRemove) {
			BuildingToRemove->K2_DestroyActor();
		}
	}
	BuildingsToRemove.Free();

	ABuildingActor* BuildingActor = World->SpawnActorUnfinished(BuildingClassData.BuildingClass.Class, BuildLoc, BuildRot, This)->Cast<ABuildingActor>();
	if (!BuildingActor) {
		Log("ServerCreateBuildingActor: Failed to spawn building actor!");
		return;
	}
	BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, This, true, nullptr);
	World->FinishSpawnActor(BuildingActor, BuildLoc, BuildRot);

	BuildingActor->CurrentBuildingLevel = BuildingClassData.UpgradeLevel;
	BuildingActor->OnRep_CurrentBuildingLevel();

	ABuildingSMActor* BuildingSMActor = BuildingActor->Cast<ABuildingSMActor>();
	if (BuildingSMActor) {
		BuildingSMActor->bMirrored = bMirrored;
	}

	BuildingActor->bPlayerPlaced = true;

	AFortPlayerStateAthena* PlayerState = This->PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerState) {
		BuildingActor->Team = PlayerState->TeamIndex;
	}
	BuildingActor->TeamIndex = BuildingActor->Team;

	This->PayBuildableClassPlacementCost(&BuildingClassData);
}

bool AFortPlayerController::CanAffordToPlaceBuildableClass(FBuildingClassData* ClassToBuildData) {
	bool (*&CanAffordToPlaceBuildableClassInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(CanAffordToPlaceBuildableClassInternal)(VTable[Finder::FindAFortPlayerController_CanAffordToPlaceBuildableClassVFT()]);
	return CanAffordToPlaceBuildableClassInternal(this, ClassToBuildData);
}

int32 AFortPlayerController::PayBuildableClassPlacementCost(FBuildingClassData* ClassToBuildData) {
	int32(*&PayBuildableClassPlacementCostInternal)(AFortPlayerController * This, FBuildingClassData * ClassToBuildData) = decltype(PayBuildableClassPlacementCostInternal)(VTable[Finder::FindAFortPlayerController_PayBuildableClassPlacementCostVFT()]);
	return PayBuildableClassPlacementCostInternal(this, ClassToBuildData);
}

void AFortPlayerController::ServerBeginEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerBeginEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToEdit) {
		Log("ServerBeginEditingBuildingActor: BuildingActorToEdit is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = This->PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerBeginEditingBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team && BuildingActorToEdit->bPlayerPlaced) {
			Log("ServerBeginEditingBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToEdit->SetEditingPlayer(PlayerState);

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerBeginEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	if (!MyFortPawn->CurrentWeapon->Cast<AFortWeap_EditingTool>()) {
		FFortItemEntry* ItemEntry = This->WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
		if (ItemEntry) {
			This->ServerExecuteInventoryItem(This, ItemEntry->ItemGuid);
		}
		else {
			Log("ServerBeginEditingBuildingActor: Failed to find edit tool in inventory!");
			return;
		}
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->CurrentWeapon->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerBeginEditingBuildingActor: Current weapon is not the editing tool!");
		return;
	}

	EditingTool->EditActor = BuildingActorToEdit;
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerEditBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToEdit, TSubclassOf<ABuildingSMActor> NewBuildingClass, int32 RotationIterations, bool bMirrored) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerEditBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToEdit) {
		Log("ServerEditBuildingActor: BuildingActorToEdit is null!");
		return;
	}

	if (!NewBuildingClass) {
		Log("ServerEditBuildingActor: NewBuildingClass is null!");
		return;
	}

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	AFortPlayerStateZone* PlayerState = This->PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerEditBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	if (BuildingActorToEdit->EditingPlayer != PlayerState) {
		Log("ServerEditBuildingActor: Player is not the editing player for this building!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team && BuildingActorToEdit->bPlayerPlaced) {
			Log("ServerEditBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
	}

	BuildingActorToEdit->SetEditingPlayer(nullptr);

	ABuildingSMActor* ReplacedBuildingActor = BuildingActorToEdit->ReplaceBuildingActor(
		EBuildingReplacementType::GetBRT_Edited(),
		NewBuildingClass.Class,
		BuildingActorToEdit->CurrentBuildingLevel,
		RotationIterations,
		bMirrored,
		This
	);

	if (!ReplacedBuildingActor) {
		Log("ServerEditBuildingActor: Failed to replace building actor!");
		return;
	}

	ReplacedBuildingActor->bPlayerPlaced = true;
	if (PlayerStateAthena) {
		ReplacedBuildingActor->Team = PlayerStateAthena->TeamIndex;
		ReplacedBuildingActor->TeamIndex = PlayerStateAthena->TeamIndex;
	}

	if (Version::Fortnite_Version <= 1.72) {
		ServerEndEditingBuildingActor(This, ReplacedBuildingActor);
	}
}

void AFortPlayerController::ServerEndEditingBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToStopEditing) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerEndEditingBuildingActor: World is null!");
		return;
	}

	if (!BuildingActorToStopEditing) {
		Log("ServerEndEditingBuildingActor: BuildingActorToStopEditing is null!");
		return;
	}

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	BuildingActorToStopEditing->SetEditingPlayer(nullptr);

	FFortItemEntry* EditToolEntry = This->WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
	if (!EditToolEntry) {
		Log("ServerEndEditingBuildingActor: Failed to find edit tool in inventory!");
		return;
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->FindWeapon(EditToolEntry->ItemGuid)->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerEndEditingBuildingActor: Failed to find editing tool weapon instance!");
		return;
	}

	EditingTool->EditActor = nullptr;
	EditingTool->OnRep_EditActor();
}

void AFortPlayerController::ServerRemoveInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, uint8 StateValueType) {
	if (!This->WorldInventory) {
		Log("ServerRemoveInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerRemoveInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	bool bModified = false;
	for (int32 i = ItemEntry->StateValues.Num() - 1; i >= 0; i--) {
		FFortItemEntryStateValue& StateValue = ItemEntry->StateValues.GetWithSize(i, FFortItemEntryStateValue::GetSize());
		if (StateValue.StateType == StateValueType) {
			ItemEntry->StateValues.RemoveAt(i, FFortItemEntryStateValue::GetSize());
			bModified = true;
		}
	}

	if (bModified) {
		This->WorldInventory->Update(ItemEntry);
	}
}

void AFortPlayerController::ServerSetInventoryStateValue(AFortPlayerController* This, FGuid& ItemGuid, FFortItemEntryStateValue& StateValue) {
	Log("AFortPlayerController::ServerSetInventoryStateValue Called!");
	Log("PlayerController: " + This->GetFullName());

	if (!This->WorldInventory) {
		Log("ServerSetInventoryStateValue: WorldInventory is null!");
		return;
	}

	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("ServerSetInventoryStateValue: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	ItemEntry->StateValues.Add(StateValue, FFortItemEntryStateValue::GetSize());
	This->WorldInventory->Update(ItemEntry);
}

UFortRegisteredPlayerInfo* AFortPlayerController::GetRegisteredPlayerInfo() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetRegisteredPlayerInfo");

	return const_cast<AFortPlayerController*>(this)->Call<UFortRegisteredPlayerInfo*>(Func);
}

UFortQuestManager* AFortPlayerController::GetQuestManager(ESubGame SubGame) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetQuestManager");

	return const_cast<AFortPlayerController*>(this)->Call<UFortQuestManager*>(Func, SubGame);
}

void AFortPlayerController::ServerRepairBuildingActor(ABuildingSMActor* BuildingActorToRepair) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerRepairBuildingActor: World is null!");
		return;
	}

	if (BuildingActorToRepair->EditingPlayer) {
		return;
	}

	if (int32 RepairCost = PayBuildingRepairCost(BuildingActorToRepair)) {
		BuildingActorToRepair->RepairBuilding(this, RepairCost);
	}
}

void AFortPlayerController::execServerRepairBuildingActor(AFortPlayerController* Context, FFrame& Stack) {
	struct FortPlayerController_ServerRepairBuildingActor
	{
	public:
		ABuildingSMActor* BuildingActorToRepair;
	};
	FortPlayerController_ServerRepairBuildingActor* Parms = (FortPlayerController_ServerRepairBuildingActor*)Stack.Locals;

	execServerRepairBuildingActorOG(Context, Stack);
	if (Version::Fortnite_Version >= 1.8 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		Context->ServerRepairBuildingActor(Parms->BuildingActorToRepair);
	}
}

int32 AFortPlayerController::PayBuildingRepairCost(ABuildingSMActor* BuildingToRepair) {
	int32(*PayBuildingRepairCostInternal)(AFortPlayerController * This, ABuildingSMActor * BuildingToRepair) = decltype(PayBuildingRepairCostInternal)(ImageBase + Finder::FindAFortPlayerController_PayBuildingRepairCost());
	return PayBuildingRepairCostInternal(this, BuildingToRepair);
}

void AFortPlayerController::ServerPlayEmoteItem(AFortPlayerController* This, UFortMontageItemDefinitionBase* EmoteAsset, float EmoteRandomNumber) {
	ServerPlayEmoteItemOG(This, EmoteAsset);
	if (Version::Fortnite_Version <= 1.82 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return;
	}

	if (!EmoteAsset) {
		Log("ServerPlayEmoteItem: EmoteAsset is null!");
		return;
	}

	if (!This->MyFortPawn) {
		Log("ServerPlayEmoteItem: MyFortPawn is null!");
		return;
	}

	UFortAbilitySystemComponent* ASC = This->MyFortPawn->AbilitySystemComponent;
	if (!ASC) {
		Log("ServerPlayEmoteItem: AbilitySystemComponent is null!");
		return;
	}

	UFortGameplayAbility* EmoteAbility = nullptr;
	if (UAthenaSprayItemDefinition* SprayItemDef = EmoteAsset->Cast<UAthenaSprayItemDefinition>()) {
		UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C");
		if (Ability) {
			EmoteAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
		}
	}
	else if (UAthenaToyItemDefinition* ToyItemDef = EmoteAsset->Cast<UAthenaToyItemDefinition>()) {
		// Toys (golf ball, beach ball, etc.) are dance-family items but carry their
		// own spawn ability. Falling through to GAB_Emote_Generic only plays the
		// throw animation and never spawns the toy actor (issue #86), so activate
		// the item's ToySpawnAbility instead.
		UClass* ToyAbility = ToyItemDef->ToySpawnAbility.Get();
		if (ToyAbility) {
			EmoteAbility = ToyAbility->GetDefaultObj()->Cast<UFortGameplayAbility>();
		}
	}
	else if (UAthenaDanceItemDefinition* DanceItemDef = EmoteAsset->Cast<UAthenaDanceItemDefinition>()) {
		UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
		if (Ability) {
			EmoteAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
		}
	}

	if (EmoteAbility) {
		FGameplayAbilitySpec* EmoteAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(EmoteAbility, 1, -1, EmoteAsset);
		ASC->GiveAbilityAndActivateOnce(EmoteAbilitySpec, nullptr);
	}
}

AActor* AFortPlayerController::SpawnToyInstance(TSubclassOf<AActor> ToyClass, FTransform& SpawnPosition) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		return nullptr;
	}

	AActor* NewToy = World->SpawnActor(ToyClass.Class, SpawnPosition, this);

	return NewToy;
}

void AFortPlayerController::execSpawnToyInstance(AFortPlayerController* Context, FFrame& Stack, AActor** Result) {
	// The toy spawn ability calls SpawnToyInstance from its Blueprint graph on the
	// server. The native implementation does not create the toy actor on a
	// Core-hosted server, so read the params, spawn the toy ourselves and hand it
	// back as the return value (issue #86).
	TSubclassOf<AActor> ToyClass;
	FTransform SpawnPosition;

	Stack.StepCompiledIn(&ToyClass);
	Stack.StepCompiledIn(&SpawnPosition);
	Stack.IncrementCode();

	if (!Context || !ToyClass) {
		return;
	}

	*Result = Context->SpawnToyInstance(ToyClass, SpawnPosition);
}

void AFortPlayerController::ServerPlaySprayItem(AFortPlayerController* This, UAthenaSprayItemDefinition* SprayAsset) {
	if (!SprayAsset) {
		Log("ServerPlaySprayItem: SprayAsset is null!");
		return;
	}

	if (!This->MyFortPawn) {
		Log("ServerPlaySprayItem: MyFortPawn is null!");
		return;
	}

	UFortAbilitySystemComponent* ASC = This->MyFortPawn->AbilitySystemComponent;
	if (!ASC) {
		Log("ServerPlaySprayItem: AbilitySystemComponent is null!");
		return;
	}

	UFortGameplayAbility* SprayAbility = nullptr;
	if (UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Sprays/GAB_Spray_Generic.GAB_Spray_Generic_C")) {
		SprayAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
	}

	if (SprayAbility) {
		FGameplayAbilitySpec* EmoteAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(SprayAbility, 1, -1, SprayAsset);
		ASC->GiveAbilityAndActivateOnce(EmoteAbilitySpec, nullptr);
	}
}

void AFortPlayerController::GetPlayerViewPoint(AFortPlayerController* This, FVector& out_Location, FRotator& out_Rotation) {
	GetPlayerViewPointOG(This, out_Location, out_Rotation);
	if (Version::Fortnite_Version <= 1.72) {
		return;
	}

	AActor* ViewTarget = This->GetViewTarget();
	AFortPlayerPawn* ViewTargetFortPawn = ViewTarget->Cast<AFortPlayerPawn>();

	if (ViewTarget) {
		This->GetActorEyesViewPoint(&out_Location, &out_Rotation);
	}
}

FUniqueNetIdRepl AFortPlayerController::GetGameAccountId() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetGameAccountId");

	return const_cast<AFortPlayerController*>(this)->Call<FUniqueNetIdRepl>(Func);
}

void AFortPlayerController::ServerAttemptInteract(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType, UObject* OptionalObjectData) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInteract: World is null!");
		return;
	}

	if (!ReceivingActor) {
		Log("AFortPlayerController::ServerAttemptInteract: ReceivingActor is null!");
		return;
	}

	UFortQuestManager* QuestManager = This->GetQuestManager(ESubGame::GetAthena());

	if (ABuildingItemCollectorActor* ItemCollector = ReceivingActor->Cast<ABuildingItemCollectorActor>()) {
		ItemCollector->ControllingPlayer = This;
		ItemCollector->bCurrentInteractionSuccess = ItemCollector->GrantOutput();
		ItemCollector->ControllingPlayer = nullptr;
	}

	//Log("ReceivingActor: " + ReceivingActor->GetName().ToString());
	ReceivingActor->ForceNetUpdate();
}

void AFortPlayerController::execServerAttemptInteract(AFortPlayerController* Context, FFrame& Stack) {
	struct FortPlayerController_ServerAttemptInteract
	{
	public:
		AActor* ReceivingActor;
		UPrimitiveComponent* InteractComponent;
		uint8 InteractType;
		UObject* OptionalObjectData;
	};
	FortPlayerController_ServerAttemptInteract* Parms = (FortPlayerController_ServerAttemptInteract*)Stack.Locals;

	ServerAttemptInteract(Context, Parms->ReceivingActor, Parms->InteractComponent, Parms->InteractType, Parms->OptionalObjectData);
	execServerAttemptInteractOG(Context, Stack);
}

FVector AFortPlayerController::GetDropFinalLocation()
{
	if (!MyFortPawn)
		return FVector{};

	FVector FinalLoc = MyFortPawn->K2_GetActorLocation();
	const FVector Forward = MyFortPawn->GetActorForwardVector();

	FinalLoc += Forward * 450.0f;
	FinalLoc.Z += 50.0f;

	const float AngleDeg = UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f);
	const float AngleRad = AngleDeg * 0.017453292519943295f;
	const float Radius = UKismetMathLibrary::RandomFloatInRange(0.0f, 100.0f);

	FinalLoc.X += std::cos(AngleRad) * Radius;
	FinalLoc.Y += std::sin(AngleRad) * Radius;

	return FinalLoc;
}

void AFortPlayerController::ClientExecuteInventoryItem(FGuid& ItemGuid, float Delay, bool bForceExecute, bool bActivateSlotAfterSettingFocused)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientExecuteInventoryItem");

	if (!Func) {
		return;
	}

	Call(Func, ItemGuid, Delay, bForceExecute, bActivateSlotAfterSettingFocused);
}

// gotta make this proper at some point
void AFortPlayerController::TogglePersonalVehicle(bool bOn) {
	Log("AFortPlayerController::TogglePersonalVehicle called with bOn = " + std::to_string(bOn));

	UFortAbilitySystemComponent* ASC = MyFortPawn ? MyFortPawn->AbilitySystemComponent : nullptr;
	if (!ASC) {
		Log("TogglePersonalVehicle: AbilitySystemComponent is null!");
		return;
	}

	UFortPersonalVehicleItemDefinition* PersonalVehicleDef = (UFortPersonalVehicleItemDefinition*)StaticLoadObject("/Game/Items/Hoverboard/VID_Hoverboard.VID_Hoverboard");
	if (!PersonalVehicleDef) {
		Log("TogglePersonalVehicle: Failed to load personal vehicle definition!");
		return;
	}

	UClass* PersonalVehicleAbilityClass = PersonalVehicleDef->PersonalVehicleAbility.Get();
	if (!PersonalVehicleAbilityClass) {
		Log("TogglePersonalVehicle: PersonalVehicleAbility is null!");
		return;
	}

	UFortGameplayAbility* PersonalVehicleAbility = PersonalVehicleAbilityClass->GetDefaultObj()->Cast<UFortGameplayAbility>();
	if (!PersonalVehicleAbility) {
		Log("TogglePersonalVehicle: Failed to get default object for personal vehicle ability!");
		return;
	}

	if (PersonalVehicleAbility) {
		if (bOn) {
			FGameplayAbilitySpec* PersonalVehicleAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(PersonalVehicleAbility, 1, -1, nullptr);
			ASC->GiveAbilityAndActivateOnce(PersonalVehicleAbilitySpec, nullptr);
		}
		else {
			ASC->EndAbility(PersonalVehicleAbility);
		}
	}
}

void AFortPlayerController::TogglePersonalVehicleHook(AFortPlayerController* This, bool bOn) {
	if (!This) {
		Log("TogglePersonalVehicleHook: This is null!");
		return;
	}

	if (AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>()) {
		FortPCAthena->TogglePersonalVehicle(bOn);
	}

	This->TogglePersonalVehicle(bOn);
}

void AFortPlayerController::execTogglePersonalVehicle(AFortPlayerController* Context, FFrame& Stack) {
	bool bOn = false;
	Stack.StepCompiledIn(&bOn);
	Stack.IncrementCode();

	TogglePersonalVehicleHook(Context, bOn);
}

bool AFortPlayerController::IsPersonalVehicleActive() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsPersonalVehicleActive");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

bool AFortPlayerController::IsPersonalVehicleAvailable() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsPersonalVehicleAvailable");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

bool AFortPlayerController::IsUsingPersonalVehicle() const {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsUsingPersonalVehicle");

	return const_cast<AFortPlayerController*>(this)->Call<bool>(Func);
}

void AFortPlayerController::ServerModifyStat(FName StatName, int32 Amount, uint8 ModType, bool bForceStatSave) {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerModifyStat");

	return Call(Func, StatName, Amount, ModType, bForceStatSave);
}

void AFortPlayerController::OnReadyToStartMatchVFT() {
	void (*&OnReadyToStartMatchInternal)(AFortPlayerController*) = decltype(OnReadyToStartMatchInternal)(VTable[Finder::FindAFortPlayerController_OnReadyToStartMatchVFT()]);
	return OnReadyToStartMatchInternal(this);
}
