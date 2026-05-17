#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/Cheat/FortCheatManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/FortItem/FortWorldItem.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/Info/FortRegisteredPlayerInfo.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/BuildingActor/BuildingSMActor.h"

void AFortPlayerController::ClientForceProfileQuery()
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientForceProfileQuery"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ClientForceProfileQueryInternal)(AFortPlayerController*) = decltype(ClientForceProfileQueryInternal)(VTable[VTableIdx]);
		return ClientForceProfileQueryInternal(this);
	}
}

void AFortPlayerController::OnReadyToStartMatch(AFortPlayerController* This) {
	OnReadyToStartMatchOG(This);

	if ((!This->QuickBars || !This->ClientQuickBars))
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

		if (IsUsingOldQuickBars()) {
			if (!QuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					ClientQuickBars = NewQuickBars->Cast<AFortQuickBars>();
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

void AFortPlayerController::ServerCheat(AFortPlayerController* This, FString* Msg) {
	std::string Command = Msg->ToString();
	Log("ServerCheat (" + This->GetName().ToString() + "): [" + Command + "]");
	
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

	if (Parser.IsCommand("Help"))
	{
		This->ClientMessage("=== Available Commands ===");
		This->ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
		This->ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
		This->ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
		This->ClientMessage("DumpInventory - Dump Inventory Stats");
		This->ClientMessage("SpawnBot [bSpawnAtPlayer] - Spawns a bot at the player's location or playerstart.");
		This->ClientMessage("SetHealth <Health> - Sets the player's health.");
		This->ClientMessage("SetShield <Shield> - Sets the player's shield.");
		This->ClientMessage("SetMaxHealth <MaxHealth> - Sets the player's max health.");
		This->ClientMessage("SetMaxShield <MaxShield> - Sets the player's max shield.");
	}
	else if (Parser.IsCommand("GiveItem")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: GiveItem <ItemDefinitionName> [Count]");
			return;
		}

		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}
		
		std::string ItemDefName = Parser.GetArg(0);
		int32 Count = Parser.GetArgInt(1, 1);

		UObject* ItemObj;
		if (ItemDefName.contains("/")) {
			if (ItemDefName.starts_with("FortniteGame/"))
			{
				ItemDefName = "/Game/" + ItemDefName.substr(strlen("FortniteGame/"));
			}

			size_t contentPos = ItemDefName.find("/Content/");
			if (contentPos != std::string::npos)
			{
				if (ItemDefName.contains("/Game/Content/"))
				{
					ItemDefName.replace(ItemDefName.find("/Game/Content/"), strlen("/Game/Content/"), "/Game/");
				}
				else
				{
					size_t contentPos = ItemDefName.find("/Content/");
					ItemDefName = ItemDefName.substr(0, contentPos)
						+ "/Game/"
						+ ItemDefName.substr(contentPos + strlen("/Content/"));
				}
			}

			if (!ItemDefName.contains("."))
			{
				size_t lastSlash = ItemDefName.find_last_of('/');
				if (lastSlash != std::string::npos)
				{
					std::string className = ItemDefName.substr(lastSlash + 1);
					ItemDefName += "." + className;
				}
				else
				{
					This->ClientMessage("Invalid ItemDefinition path: " + ItemDefName);
					return;
				}
			}

			ItemObj = StaticLoadObject(ItemDefName);
		}
		else {
			ItemObj = FUObjectArray::FindObjectFast(ItemDefName);
		}
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		This->WorldInventory->AddItemAndHandleOverflow(ItemDef, Count);
	}
	else if (Parser.IsCommand("SetLoadedAmmo")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SetLoadedAmmo <LoadedAmmo>");
			return;
		}

		int32 LoadedAmmo = Parser.GetArgInt(0, 30);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}
		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}

		FFortItemEntry* WeaponEntry = This->FindItemEntry(CurrentWeapon->ItemEntryGuid);
		if (!WeaponEntry) {
			This->ClientMessage("Weapon entry not found in inventory!");
			return;
		}

		WeaponEntry->LoadedAmmo = LoadedAmmo;
		This->WorldInventory->Update(WeaponEntry);
		This->ClientMessage("Set loaded ammo of current weapon to " + std::to_string(LoadedAmmo));
	}
	else if (Parser.IsCommand("GiveAmmo")) {
		int32 AmmoAmount = Parser.GetArgInt(0, 30);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}

		if (!CurrentWeapon->WeaponData) {
			This->ClientMessage("WeaponData is null!");
			return;
		}

		UFortWorldItemDefinition* ItemDef = CurrentWeapon->WeaponData->Cast<UFortWorldItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("WeaponData is not a UFortWorldItemDefinition!");
			return;
		}

		UFortWorldItemDefinition* AmmoItemDef = ItemDef->GetAmmoWorldItemDefinition_BP();
		if (!AmmoItemDef) {
			This->ClientMessage("AmmoWorldItemDefinition is null!");
			return;
		}

		FFortItemEntry* AmmoEntry = This->FindItemEntry(AmmoItemDef);
		if (AmmoEntry) {
			AmmoEntry->Count += AmmoAmount;
			This->WorldInventory->Update(AmmoEntry);
			This->ClientMessage("Added " + AmmoItemDef->GetName().ToString() + " " + std::to_string(AmmoAmount) + " ammo to existing stack.");
		}
		else {
			AmmoEntry = This->WorldInventory->AddItem(AmmoItemDef, AmmoAmount);
			if (AmmoEntry) {
				This->ClientMessage("Added new ammo entry: " + AmmoItemDef->GetName().ToString() + " x" + std::to_string(AmmoAmount));
			}
			else {
				This->ClientMessage("Failed to add ammo item: " + AmmoItemDef->GetName().ToString());
			}
		}
	} else if (Parser.IsCommand("DumpInventory")) {
		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}
		This->ClientMessage("=== Inventory Dump ===");

		This->ClientMessage("Stats: ");
		This->ClientMessage("Inventory Capacity: " + std::to_string(This->WorldInventory->GetInventoryCapacity()));
		This->ClientMessage("Inventory Used: " + std::to_string(This->WorldInventory->GetInventoryUsed()));

		This->ClientMessage("Items: ");
		for (UFortWorldItem* ItemEntry : This->WorldInventory->Inventory.ItemInstances) {
			std::string ItemName = ItemEntry->ItemEntry.ItemDefinition ? ItemEntry->ItemEntry.ItemDefinition->GetName().ToString() : "Unknown";
			int32 Count = ItemEntry->ItemEntry.Count;
			std::string FormattedGuid = ItemEntry->ItemEntry.ItemGuid.FormatGuid();

			This->ClientMessage("Item: " + ItemName + ", Count: " + std::to_string(Count) + ", GUID: " + FormattedGuid);
		}

		This->ClientMessage("=== End of Inventory Dump ===");
	}
	else if (Parser.IsCommand("SpawnBot")) {
		bool bSpawnAtPlayer = Parser.GetArgBool(0, false);
		
		bool bSuccessfulSpawn = GameMode->SpawnPlayerBot(bSpawnAtPlayer ? This->MyFortPawn : nullptr);
		if (bSuccessfulSpawn) {
			This->ClientMessage("Bot spawned successfully!");
		}
		else {
			This->ClientMessage("Failed to spawn bot.");
		}
	} else if (Parser.IsCommand("SetHealth")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SetHealth <Health>");
			return;
		}

		float Health = Parser.GetArgFloat(0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetHealth(Health);

		This->ClientMessage("Set health to " + std::to_string(Health));
	} else if (Parser.IsCommand("SetShield")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SetShield <Shield>");
			return;
		}

		float Shield = Parser.GetArgFloat(0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetShield(Shield);

		This->ClientMessage("Set shield to " + std::to_string(Shield));
	} else if (Parser.IsCommand("SetMaxHealth")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SetMaxHealth <MaxHealth>");
			return;
		}

		float MaxHealth = Parser.GetArgFloat(0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetMaxHealth(MaxHealth);

		This->ClientMessage("Set max health to " + std::to_string(MaxHealth));
	} else if (Parser.IsCommand("SetMaxShield")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SetMaxShield <MaxShield>");
			return;
		}

		float MaxShield = Parser.GetArgFloat(0, 100.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		This->MyFortPawn->SetMaxShield(MaxShield);

		This->ClientMessage("Set max shield to " + std::to_string(MaxShield));
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

	struct FortPlayerController_ClientReportDamagedResourceBuilding
	{
	public:
		ABuildingSMActor* BuildingSMActor;
		uint8 PotentialResourceType;
		int32 PotentialResourceCount;
		bool bDestroyed;
		bool bJustHitWeakspot;
	};

	FortPlayerController_ClientReportDamagedResourceBuilding Parms{};

	Parms.BuildingSMActor = BuildingSMActor;
	Parms.PotentialResourceType = PotentialResourceType;
	Parms.PotentialResourceCount = PotentialResourceCount;
	Parms.bDestroyed = bDestroyed;
	Parms.bJustHitWeakspot = bJustHitWeakspot;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerController::ServerAttemptInventoryDrop(AFortPlayerController* This, FGuid& ItemGuid, int Count, bool bTrash) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: World is null!");
		return;
	}

	if (!This->MyFortPawn) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: MyFortPawn is null!");
		return;
	}

	FVector PawnLocation = This->MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerAttemptInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count) + ", bTrash: " + std::to_string(bTrash));
	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (ItemEntry) {
		if (ItemEntry->ItemDefinition) {
			if (!bTrash) {
				This->WorldInventory->SpawnPickupFromEntry(*ItemEntry);
			}
			This->WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
		}
	}
}

void AFortPlayerController::ClientForceUpdateQuickbar(uint8 QuickbarToRefresh)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientForceUpdateQuickbar");

	struct FortPlayerController_ClientForceUpdateQuickbar
	{
	public:
		uint8 QuickbarToRefresh;
	};

	FortPlayerController_ClientForceUpdateQuickbar Parms{};

	Parms.QuickbarToRefresh = QuickbarToRefresh;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerController::OnRep_QuickBar()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_QuickBar");

	ProcessEvent(Func, nullptr);
}

bool AFortPlayerController::IsUsingOldQuickBars() {
	static uintptr_t ClientQuickBarsOffset = StaticClass()->GetPropertyOffset("ClientQuickBars");

	return ClientQuickBarsOffset == -1;
}

void AFortPlayerController::ServerClientPawnLoaded(AFortPlayerController* This, bool bIsPawnLoaded)
{
	ServerClientPawnLoadedOG(This, bIsPawnLoaded);
	AFortPlayerState* PlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerState>() : nullptr;
	if (!PlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerPawn* MyFortPawn = This->MyFortPawn;

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();

	if (bIsPawnLoaded) {
		if (Version::Fortnite_Version <= 1.72) {
			if (FortPCAthena && MyFortPawn) {
				if (FortPCAthena->StrongMyHero && FortPCAthena->StrongMyHero->CharacterParts.Num() > 0) {
					for (UCustomCharacterPart* CharacterPart : FortPCAthena->StrongMyHero->CharacterParts) {
						MyFortPawn->ServerChoosePart(CharacterPart, CharacterPart->CharacterPartType);
					}
				}
			}

			PlayerState->OnRep_CharacterParts();
		}
		else {
			// ApplyCharacterCustomization
		}
	}
}

// This is stripped but idk what to do for it
void AFortPlayerController::ServerSpotActor(AFortPlayerController* This, AActor* NewlySpottedActor)
{
	ServerSpotActorOG(This, NewlySpottedActor);

	Log("ServerSpotActor Called!");
}

bool AFortPlayerController::RemoveInventoryItem(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval)
{
	if (!This || Count <= 0) {
		return false;
	}

	static auto InterfaceOffset = StaticClass()->SuperStruct->PropertiesSize + (Version::Engine_Version >= 4.27 ? 16 : 8);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(__int64(This) - InterfaceOffset); // this is so wierd
	if (!PlayerController) {
		Log("RemoveInventoryItem: Failed to get PlayerController from interface pointer!");
		return false;
	}
	if (PlayerController->IsA(AFortPlayerState::StaticClass())) {
		AFortPlayerState* PlayerState = PlayerController->Cast<AFortPlayerState>();
		PlayerController = (AFortPlayerController*)PlayerState->Owner;
	}

	if (!PlayerController->IsA(AFortPlayerController::StaticClass())) {
		Log("RemoveInventoryItem: PlayerController is not a AFortPlayerController!");
		return false;
	}

	AFortInventory* Inventory = PlayerController->WorldInventory;
	if (!Inventory) {
		return false;
	}

	if (!ItemGuid.IsValid()) {
		return false;
	}

	Inventory->RemoveItem(ItemGuid, Count);

	return true;
}

void AFortPlayerController::ServerCreateBuildingActorOld(AFortPlayerController* This, FBuildingClassData& BuildingClassData, FVector& BuildLoc, FRotator& BuildRot, bool bMirrored) {
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
		Log("ServerCreateBuildingActor: Cannot afford to place building!");
		return;
	}

	TArray<ABuildingActor*> BuildingsToRemove;
	uint8 OptionalAdjustment;
	if (UFortKismetLibrary::CanPlaceBuildableClassInStructuralGrid(World, BuildingClassData.BuildingClass.Get(), BuildLoc, BuildRot, bMirrored, &BuildingsToRemove, &OptionalAdjustment)) {
		Log("ServerCreateBuildingActor: Cannot place building at location!");
		return;
	}

	for (ABuildingActor* BuildingToRemove : BuildingsToRemove) {
		if (BuildingToRemove) {
			BuildingToRemove->K2_DestroyActor();
		}
	}
	BuildingsToRemove.Free();

	ABuildingActor* BuildingActor = World->SpawnActor(BuildingClassData.BuildingClass.Class, BuildLoc, BuildRot, This)->Cast<ABuildingActor>();
	if (!BuildingActor) {
		Log("ServerCreateBuildingActor: Failed to spawn building actor!");
		return;
	}
	BuildingActor->InitializeKismetSpawnedBuildingActor(BuildingActor, This, true, nullptr);

	BuildingActor->bPlayerPlaced = true;

	AFortPlayerStateAthena* PlayerState = This->PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerState) {
		BuildingActor->Team = PlayerState->TeamIndex;
		BuildingActor->TeamIndex = PlayerState->TeamIndex;
	}

	ABuildingSMActor* BuildingSMActor = BuildingActor->Cast<ABuildingSMActor>();
	if (BuildingSMActor) {
		BuildingSMActor->bMirrored = bMirrored;
	}

	This->PayBuildableClassPlacementCost(&BuildingClassData);

	BuildingActor->ForceNetUpdate();
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
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team) {
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

	EditingTool->ForceNetUpdate();
	BuildingActorToEdit->ForceNetUpdate();
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
		if (PlayerStateAthena->TeamIndex != BuildingActorToEdit->Team) {
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

	ReplacedBuildingActor->InitializeKismetSpawnedBuildingActor(ReplacedBuildingActor, This, true, BuildingActorToEdit);

	ReplacedBuildingActor->bPlayerPlaced = true;
	if (PlayerStateAthena) {
		ReplacedBuildingActor->Team = PlayerStateAthena->TeamIndex;
		ReplacedBuildingActor->TeamIndex = PlayerStateAthena->TeamIndex;
	}

	ReplacedBuildingActor->SetEditingPlayer(PlayerState);

	FFortItemEntry* EditToolEntry = This->WorldInventory->FindItemEntry(UFortEditToolItemDefinition::StaticClass());
	if (!EditToolEntry) {
		Log("ServerEditBuildingActor: Failed to find edit tool in inventory!");
		return;
	}

	AFortWeap_EditingTool* EditingTool = MyFortPawn->FindWeapon(EditToolEntry->ItemGuid)->Cast<AFortWeap_EditingTool>();
	if (!EditingTool) {
		Log("ServerEditBuildingActor: Failed to find editing tool weapon instance!");
		return;
	}

	EditingTool->EditActor = ReplacedBuildingActor;
	EditingTool->OnRep_EditActor();

	ReplacedBuildingActor->ForceNetUpdate();
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

	AFortPlayerStateZone* PlayerState = This->PlayerState->Cast<AFortPlayerStateZone>();
	if (!PlayerState) {
		Log("ServerEndEditingBuildingActor: PlayerState is null or not AFortPlayerStateZone!");
		return;
	}

	AFortPawn* MyFortPawn = This->MyFortPawn;
	if (!MyFortPawn) {
		Log("ServerEndEditingBuildingActor: MyFortPawn is null!");
		return;
	}

	if (BuildingActorToStopEditing->EditingPlayer != PlayerState) {
		Log("ServerEndEditingBuildingActor: Player is not the editing player for this building!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (PlayerStateAthena) {
		if (PlayerStateAthena->TeamIndex != BuildingActorToStopEditing->Team) {
			Log("ServerEndEditingBuildingActor: Player is not on the same team as the building, cannot edit. Player hacking?");
			return;
		}
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

	EditingTool->ForceNetUpdate();
	BuildingActorToStopEditing->ForceNetUpdate();
}