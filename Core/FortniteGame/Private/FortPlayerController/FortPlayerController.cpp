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
	if (Finder::FindAFortPlayerController_SpawnQuickBarsVFT()) {
		void (*&SpawnQuickBarsInternal)(AFortPlayerController* This) = decltype(SpawnQuickBarsInternal)(VTable[Finder::FindAFortPlayerController_SpawnQuickBarsVFT()]);
		SpawnQuickBarsInternal(this);
	}
	else {
		if (IsUsingOldQuickBars()) {
			if (!QuickBars)
			{
				UWorld* World = GetWorld();
				if (!World) {
					return;
				}
				AActor* NewQuickBars = World->SpawnActor(AFortQuickBars::StaticClass(), FVector(), FRotator(), this);
				if (NewQuickBars && NewQuickBars->Cast<AFortQuickBars>()) {
					QuickBars = NewQuickBars->Cast<AFortQuickBars>();
				}
			}
		}
		else {
			if (!ClientQuickBars)
			{
				UWorld* World = GetWorld();
				if (!World) {
					return;
				}
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

	FCommandParser Parser(Command);

	if (Parser.IsCommand("Help"))
	{
		This->ClientMessage("=== Available Commands ===");
		This->ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
		This->ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
		This->ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
		This->ClientMessage("DumpInventory - Dump Inventory Stats");
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
			AmmoEntry = &This->WorldInventory->AddItem(AmmoItemDef, AmmoAmount)->ItemEntry;
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
					ItemEntry->Count,
					PawnLocation,
					FVector(),
					-1,
					true,
					true,
					false,
					-1,
					EFortPickupSourceTypeFlag::Player,
					EFortPickupSpawnSource::TossedByPlayer,
					This,
					false
				);
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

	return ClientQuickBarsOffset == 0;
}