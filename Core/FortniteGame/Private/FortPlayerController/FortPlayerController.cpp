#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerController.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortManager/FortCheatManager.h"
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
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"

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
					Log("Spawned QuickBars: " + QuickBars->GetName().ToString());
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
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

void AFortPlayerController::ServerCheat(AFortPlayerController* This, FString* Msg) {
	std::string Command = Msg->ToString();
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

	if (Parser.IsCommand("Help"))
	{
		This->ClientMessage("=== Available Commands ===");
		This->ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
		This->ClientMessage("ForceGiveItem <ItemDefinitionName> [Count] - Forces an item to the player's inventory. Use this if GiveItem fails!");
		This->ClientMessage("SpawnPickup <ItemDefinitionName> [Count] - Spawns a pickup at the player's location.");
		This->ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
		This->ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
		This->ClientMessage("DumpInventory - Dump Inventory Stats");
		This->ClientMessage("SpawnBot [bSpawnAtPlayer] - Spawns a bot at the player's location or playerstart.");
		This->ClientMessage("SetHealth <Health> - Sets the player's health.");
		This->ClientMessage("SetShield <Shield> - Sets the player's shield.");
		This->ClientMessage("SetMaxHealth <MaxHealth> - Sets the player's max health.");
		This->ClientMessage("SetMaxShield <MaxShield> - Sets the player's max shield.");
		This->ClientMessage("SpawnActor <ActorClassName> [bSetOwnerAsThis] - Spawns an actor");
		This->ClientMessage("ClearEquippedItem - Clears the currently equipped item.");
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

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		AFortPlayerPawn* Pawn = This->Pawn->Cast<AFortPlayerPawn>();
		if (!Pawn) {
			This->ClientMessage("Pawn is null!");
			return;
		}

		FVector FinalLoc = Pawn->K2_GetActorLocation();
		FVector ForwardVector = Pawn->GetActorForwardVector();

		ForwardVector.Z = 0.0f;

		float SizeSq = ForwardVector.SizeSquared();
		if (SizeSq > 0.0001f) {
			float Size = sqrtf(SizeSq);
			ForwardVector.X /= Size;
			ForwardVector.Y /= Size;
		}

		FinalLoc = FinalLoc + ForwardVector * 450.f;
		FinalLoc.Z += 50.f;

		const float RandomAngleVariation = ((float)rand() * 0.00109866634f) - 18.f;
		const float FinalAngle = RandomAngleVariation * 0.017453292519943295f;

		FinalLoc.X += cos(FinalAngle) * 100.f;
		FinalLoc.Y += sin(FinalAngle) * 100.f;

		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemDef,
			Count,
			FinalLoc,
			FVector(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetOther(),
			EFortPickupSpawnSource::GetUnset(),
			This,
			false
		);

		if (Pickup) {
			FVector ZeroVector = { 0, 0, 0 };
			AFortPlayerPawn::ServerHandlePickup(Pawn, Pickup, Pickup->PickupLocationData.FlyTime, ZeroVector, true);
			This->ClientMessage("Given Item: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
		}
	}
	else if (Parser.IsCommand("ForceGiveItem")) {
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

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		This->WorldInventory->AddItem(ItemDef, Count);
	}
	else if (Parser.IsCommand("SpawnPickup")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SpawnPickup <ItemDefinitionName> [Count]");
			return;
		}

		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		std::string ItemDefName = Parser.GetArg(0);
		int32 Count = Parser.GetArgInt(1, 1);

		UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
		if (!ItemObj) {
			This->ClientMessage("ItemDefinition not found: " + ItemDefName);
			return;
		}

		UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
		if (!ItemDef) {
			This->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
			return;
		}

		UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemDef,
			Count,
			This->Pawn->K2_GetActorLocation(),
			FVector(),
			-1,
			true,
			true,
			false,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetUnset(),
			nullptr,
			false
		);
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
	else if (Parser.IsCommand("SpawnActor")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: SpawnActor <ActorClassName>");
			return;
		}

		std::string ActorClassName = Parser.GetArg(0);
		bool bSetOwnerAsThis = Parser.GetArgBool(1, false);
		FVector Location = This->Pawn ? This->Pawn->K2_GetActorLocation() : FVector();
		FRotator Rotation = This->Pawn ? This->Pawn->K2_GetActorRotation() : FRotator();

		UObject* ActorClassObj = Utils::GetObjectFromString(ActorClassName, EClassCastFlags::CASTCLASS_UClass);
		if (!ActorClassObj) {
			This->ClientMessage("Actor class not found: " + ActorClassName);
			return;
		}

		UClass* ActorClass = ActorClassObj->Cast<UClass>();
		if (!ActorClass) {
			This->ClientMessage("Object is not a UClass: " + ActorClassObj->GetName().ToString());
			return;
		}

		AActor* NewActor = World->SpawnActor(ActorClass, Location, Rotation, bSetOwnerAsThis ? This : nullptr);
		if (NewActor) {
			This->ClientMessage("Spawned actor: " + NewActor->GetName().ToString());
		}
		else {
			This->ClientMessage("Failed to spawn actor of class: " + ActorClass->GetName().ToString());
		}
	}
	else if (Parser.IsCommand("ClearEquippedItem")) {
		if (!This->WorldInventory) {
			This->ClientMessage("WorldInventory is null!");
			return;
		}

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		if (!This->MyFortPawn->CurrentWeapon) {
			This->ClientMessage("No currently equipped item to clear.");
			return;
		}

		This->WorldInventory->RemoveItem(This->MyFortPawn->CurrentWeapon->ItemEntryGuid);
		This->ClientMessage("Cleared currently equipped item.");
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
				AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
					World,
					ItemEntry->ItemDefinition,
					Count,
					PawnLocation,
					FVector(),
					-1,
					true,
					true,
					true,
					-1,
					EFortPickupSourceTypeFlag::GetPlayer(),
					EFortPickupSpawnSource::GetTossedByPlayer(),
					This,
					false
				);
				Pickup->PrimaryPickupItemEntry.LoadedAmmo = ItemEntry->LoadedAmmo;
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
	AFortPlayerState* FortPlayerState = This->PlayerState ? This->PlayerState->Cast<AFortPlayerState>() : nullptr;
	if (!FortPlayerState) {
		Log("ServerAcknowledgePossession: PlayerState is null or not AFortPlayerStateZone");
		return;
	}

	AFortPlayerPawn* MyFortPawn = This->MyFortPawn;

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();

	if (bIsPawnLoaded) {
		FortPlayerState->ApplyCharacterCustomization(MyFortPawn);

		if (auto KillScore = ((AFortPlayerStateAthena*)FortPCAthena->PlayerState)->KillScore)
		{
			static auto AthenaKills = UKismetStringLibrary::Conv_StringToName(L"AthenaKills");
			Utils::SetClientObservedStat((AFortPlayerPawn*)MyFortPawn, AthenaKills, KillScore);
		}
	}
}

bool AFortPlayerController::RemoveInventoryItem(AFortPlayerController* This, FGuid& ItemGuid, int32 Count, bool bForceRemoval)
{
	static auto InterfaceOffset = StaticClass()->SuperStruct->PropertiesSize + (Version::Engine_Version >= 4.27 ? 16 : 8);
	AFortPlayerController* PlayerController = (AFortPlayerController*)(__int64(This) - InterfaceOffset); // this is so wierd
	if (!PlayerController) {
		Log("RemoveInventoryItem: Failed to get PlayerController from interface pointer!");
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoval);
	}

	AFortInventory* Inventory = PlayerController->WorldInventory;
	if (!Inventory) {
		Log("RemoveInventoryItem: WorldInventory is null!");
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoval);
	}

	if (!ItemGuid.IsValid()) {
		Log("RemoveInventoryItem: Invalid ItemGuid!");
		return RemoveInventoryItemOG(This, ItemGuid, Count, bForceRemoval);
	}

	return Inventory->RemoveItem(ItemGuid, Count);
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

void AFortPlayerController::ServerAttemptInteract(AFortPlayerController* This, AActor* ReceivingActor, UPrimitiveComponent* InteractComponent, uint8 InteractType) {
	ServerAttemptInteractOG(This, ReceivingActor, InteractComponent, InteractType);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerController::ServerAttemptInteract: World is null!");
		return;
	}

	if (ReceivingActor) {
		//Log("ReceivingActor: " + ReceivingActor->GetFullName());
		ReceivingActor->ForceNetUpdate();
	}
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
	for (int32 i = 0; i < ItemEntry->StateValues.Num(); i++) {
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

	ItemEntry->StateValues.Add(StateValue);
	This->WorldInventory->Update(ItemEntry);
}

UFortRegisteredPlayerInfo* AFortPlayerController::GetRegisteredPlayerInfo() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetRegisteredPlayerInfo");

	struct FortPlayerController_GetRegisteredPlayerInfo
	{
	public:
		UFortRegisteredPlayerInfo* ReturnValue;
	};

	FortPlayerController_GetRegisteredPlayerInfo Parms{};

	const_cast<AFortPlayerController*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

UFortQuestManager* AFortPlayerController::GetQuestManager(uint8 SubGame) const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetQuestManager");

	struct FortPlayerController_GetQuestManager
	{
	public:
		uint8 SubGame;
		uint8 Pad_1[0x7];
		UFortQuestManager* ReturnValue;
	};

	FortPlayerController_GetQuestManager Parms{};

	Parms.SubGame = SubGame;

	const_cast<AFortPlayerController*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}

void AFortPlayerController::ServerRepairBuildingActor(AFortPlayerController* This, ABuildingSMActor* BuildingActorToRepair) {
	if (Version::Fortnite_Version <= 1.8 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return ServerRepairBuildingActorOG(This, BuildingActorToRepair);
	}

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerRepairBuildingActor: World is null!");
		return;
	}

	if (BuildingActorToRepair->EditingPlayer) {
		return;
	}

	if (int32 RepairCost = This->PayBuildingRepairCost(BuildingActorToRepair)) {
		BuildingActorToRepair->RepairBuilding(This, RepairCost);
	}
}

int32 AFortPlayerController::PayBuildingRepairCost(ABuildingSMActor* BuildingToRepair) {
	int32(*PayBuildingRepairCostInternal)(AFortPlayerController * This, ABuildingSMActor * BuildingToRepair) = decltype(PayBuildingRepairCostInternal)(ImageBase + Finder::FindAFortPlayerController_PayBuildingRepairCost());
	return PayBuildingRepairCostInternal(this, BuildingToRepair);
}

void AFortPlayerController::ServerPlayEmoteItem(AFortPlayerController* This, UFortMontageItemDefinitionBase* EmoteAsset) {
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

	AFortPlayerState* PlayerState = This->PlayerState->Cast<AFortPlayerState>();
	if (!PlayerState) {
		Log("ServerPlayEmoteItem: PlayerState is null or not AFortPlayerState!");
		return;
	}

	UFortAbilitySystemComponent* ASC = PlayerState->AbilitySystemComponent;
	if (!ASC) {
		Log("ServerPlayEmoteItem: AbilitySystemComponent is null!");
		return;
	}

	UFortGameplayAbility* EmoteAbility = nullptr;
	if (UAthenaDanceItemDefinition* DanceItemDef = EmoteAsset->Cast<UAthenaDanceItemDefinition>()) {
		UClass* Ability = StaticLoadObject<UClass>("/Game/Abilities/Emotes/GAB_Emote_Generic.GAB_Emote_Generic_C");
		EmoteAbility = Ability->GetDefaultObj()->Cast<UFortGameplayAbility>();
	}

	if (EmoteAbility) {
		FGameplayAbilitySpec* EmoteAbilitySpec = FGameplayAbilitySpec::ConstructAbilitySpec(EmoteAbility, 1, -1, EmoteAsset);
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

	struct FortPlayerController_GetGameAccountId
	{
	public:
		FUniqueNetIdRepl ReturnValue;
	};

	FortPlayerController_GetGameAccountId Parms{};

	const_cast<AFortPlayerController*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}