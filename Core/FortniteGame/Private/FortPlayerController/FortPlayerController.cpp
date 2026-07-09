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
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/FortAbility/FortGameplayAbility.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"

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
		This->ClientMessage("GetWeaponStats - Gets the stats of the currently equipped weapon.");
		This->ClientMessage("DumpActorsWithClass <ClassName> - Dumps all actors of a specific class.");
		This->ClientMessage("TeleportToLocation <X> <Y> <Z> - Teleports the player to a specific location.");
		This->ClientMessage("DumpCurrentLocation - Dumps the player's current location.");
		This->ClientMessage("SpawnQuickBars - Spawns the player's quickbars.");
		This->ClientMessage("DestroyQuickBars - Destroys the player's quickbars.");
		This->ClientMessage("DumpQuickBars - Dumps the player's quickbars.");
		This->ClientMessage("ServerExecuteInventoryItem <ItemGuid> - Executes an inventory item by its GUID.");
		return;
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
			This->GetDropFinalLocation(),
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
		return;
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
		return;
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
			This->GetDropFinalLocation(),
			-1,
			true,
			true,
			true,
			-1,
			EFortPickupSourceTypeFlag::GetPlayer(),
			EFortPickupSpawnSource::GetUnset(),
			This,
			false
		);
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
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
		return;
	}
	else if (Parser.IsCommand("GetWeaponStats")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}
		AFortWeapon* CurrentWeapon = This->MyFortPawn->CurrentWeapon;
		if (!CurrentWeapon) {
			This->ClientMessage("CurrentWeapon is null!");
			return;
		}
		UFortWeaponItemDefinition* WeaponDef = CurrentWeapon->WeaponData;
		if (!WeaponDef) {
			This->ClientMessage("WeaponData is null!");
			return;
		}

		This->ClientMessage("=== Weapon Stats ===");
		This->ClientMessage("Weapon Name: " + WeaponDef->GetName().ToString());
		This->ClientMessage("ClipSize: " + std::to_string(WeaponDef->GetClipSize()));
		This->ClientMessage("Durability: " + std::to_string(WeaponDef->GetDurability()));
		This->ClientMessage("=== End of Weapon Stats ===");
		return;
	}
	else if (Parser.IsCommand("DestroyTarget"))
	{
		This->CheatManager->DestroyTarget();
		return;
	}
	else if (Parser.IsCommand("DumpActorsWithClass")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: DumpActorsWithClass <ClassName>");
			return;
		}

		std::string ActorClassName = Parser.GetArg(0);

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

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(World, ActorClass, &FoundActors);

		This->ClientMessage("Found " + std::to_string(FoundActors.Num()) + " actors of class: " + ActorClass->GetName().ToString());
		This->ClientMessage("=== Actor List ===");
		for (AActor* Actor : FoundActors) {
			This->ClientMessage("");
			This->ClientMessage("Actor: " + Actor->GetName().ToString());
			FVector ActorLocation = Actor->K2_GetActorLocation();
			This->ClientMessage("Location: X=" + std::to_string(ActorLocation.X) + " Y=" + std::to_string(ActorLocation.Y) + " Z=" + std::to_string(ActorLocation.Z));
			This->ClientMessage("");
		}
		This->ClientMessage("=== End of Actor List ===");
		return;
	}
	else if (Parser.IsCommand("TeleportToLocation")) {
		if (Parser.GetArgCount() < 3)
		{
			This->ClientMessage("Usage: TeleportToLocation <X> <Y> <Z>");
			return;
		}

		float X = Parser.GetArgFloat(0, 0.0f);
		float Y = Parser.GetArgFloat(1, 0.0f);
		float Z = Parser.GetArgFloat(2, 0.0f);

		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		FVector NewLocation(X, Y, Z);

		FHitResult HitResult;
		This->MyFortPawn->K2_SetActorLocation(NewLocation, false, &HitResult, true);
		This->ClientMessage("Teleported to location: X=" + std::to_string(X) + " Y=" + std::to_string(Y) + " Z=" + std::to_string(Z));
		
		return;
	}
	else if (Parser.IsCommand("DumpCurrentLocation")) {
		if (!This->MyFortPawn) {
			This->ClientMessage("MyFortPawn is null!");
			return;
		}

		FVector CurrentLocation = This->MyFortPawn->K2_GetActorLocation();

		This->ClientMessage("=== Current Location ===");
		This->ClientMessage("X: " + std::to_string(CurrentLocation.X));
		This->ClientMessage("Y: " + std::to_string(CurrentLocation.Y));
		This->ClientMessage("Z: " + std::to_string(CurrentLocation.Z));
		This->ClientMessage("=== End of Current Location ===");

		return;
	}
	else if (Parser.IsCommand("SpawnQuickBars")) {
		This->SpawnQuickBars();
		This->SetupQuickBars();
		This->ClientMessage("Spawned QuickBars.");

		return;
	}
	else if (Parser.IsCommand("DestroyQuickBars")) {
		if (This->QuickBars) {
			This->QuickBars->K2_DestroyActor();
			This->QuickBars = nullptr;
			This->ForceNetUpdate();
			This->ClientMessage("Destroyed QuickBars.");
		}
		else if (This->ClientQuickBars) {
			This->ClientQuickBars->K2_DestroyActor();
			This->ClientQuickBars = nullptr;
			This->ForceNetUpdate();
			This->ClientMessage("Destroyed ClientQuickBars.");
		}
		else {
			This->ClientMessage("No QuickBars to destroy.");
		}

		return;
		}
	else if (Parser.IsCommand("DumpQuickBars")) {
		if (This->QuickBars) {
			This->ClientMessage("=== QuickBars ===");
			This->ClientMessage("QuickBars: " + This->QuickBars->GetName().ToString());
			This->ClientMessage("QuickBars Address: " + std::to_string((uintptr_t)This->QuickBars));
			This->ClientMessage("=== End of QuickBars ===");
		}
		else if (This->ClientQuickBars) {
			This->ClientMessage("=== ClientQuickBars ===");
			This->ClientMessage("ClientQuickBars: " + This->ClientQuickBars->GetName().ToString());
			This->ClientMessage("ClientQuickBars Address: " + std::to_string((uintptr_t)This->ClientQuickBars));
			This->ClientMessage("=== End of ClientQuickBars ===");
		}
		else {
			This->ClientMessage("No QuickBars to dump.");
		}

		return;
	}
	else if (Parser.IsCommand("ServerExecuteInventoryItem")) {
		if (Parser.GetArgCount() < 1)
		{
			This->ClientMessage("Usage: ServerExecuteInventoryItem <ItemGuid>");
			return;
		}

		std::string GuidA = Parser.GetArg(0);

		FGuid ItemGuid = FGuid::ParseGUID(GuidA);

		This->ServerExecuteInventoryItem(This, ItemGuid);

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

	FVector FinalLoc = This->MyFortPawn->K2_GetActorLocation();

	Log("AFortPlayerController::ServerAttemptInventoryDrop: Attempting to drop item with GUID: " + ItemGuid.FormatGuid() + ", Count: " + std::to_string(Count) + ", bTrash: " + std::to_string(bTrash));
	FFortItemEntry* ItemEntry = This->FindItemEntry(ItemGuid);
	if (!ItemEntry) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: ItemEntry not found for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	if (!ItemEntry->ItemDefinition) {
		Log("AFortPlayerController::ServerAttemptInventoryDrop: ItemDefinition is null for GUID: " + ItemGuid.FormatGuid());
		return;
	}

	if (!bTrash) {
		AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
			World,
			ItemEntry->ItemDefinition,
			Count,
			FinalLoc,
			This->GetDropFinalLocation(),
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
		Pickup->PrimaryPickupItemEntry.Durability = ItemEntry->Durability;
		Pickup->PrimaryPickupItemEntry.bIsDirty = true;

		Pickup->PrimaryPickupItemEntry.ReplicationKey++;
		Pickup->OnRep_PrimaryPickupItemEntry();
	}

	This->WorldInventory->RemoveItem(ItemEntry->ItemGuid, Count);
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

	return const_cast<AFortPlayerController*>(this)->Call<UFortRegisteredPlayerInfo*>(Func);
}

UFortQuestManager* AFortPlayerController::GetQuestManager(uint8 SubGame) const
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