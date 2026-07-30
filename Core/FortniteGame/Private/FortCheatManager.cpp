#include "pch.h"
#include "FortniteGame/Public/FortCheatManager.h"

#include "FortniteGame/Public/Player/FortPlayerController.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Inventory/FortQuickBarsAthena.h"
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
#include "FortniteGame/Public/Building/BuildingRift.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIEncounterInfo.h"
#include "FortniteGame/Public/AI/FortAIEncounterRiftManager.h"
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
#include "FortniteGame/Public/Athena/FortAthenaExitCraftInfo.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraftSpawner.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortMontageItemDefinitionBase.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortEnums.h"

void UFortCheatManager::AddAllScores(int32 Amount)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"AddAllScores"));

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddBuildingScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddBuildingScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddCombatScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddCombatScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddUtilityScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddUtilityScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddKillFeedMessage()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddKillFeedMessage");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func);
}

AFortPlayerController* UFortCheatManager::GetPlayerController() const
{
	return GetOuter() ? GetOuter()->Cast<AFortPlayerController>() : nullptr;
}

void UFortCheatManager::Help(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	PC->ClientMessage("=== Available Commands ===");
	PC->ClientMessage("Params can also be passed by name: Command?Param=Value?bSomeFlag");
	PC->ClientMessage("-- Items / Ammo --");
	PC->ClientMessage("GiveItem <ItemDefinitionName> [Count] - Gives an item to the player's inventory.");
	PC->ClientMessage("ForceGiveItem <ItemDefinitionName> [Count] - Forces an item into the inventory. Use this if GiveItem fails!");
	PC->ClientMessage("SpawnPickup <ItemDefinitionName> [Count] - Spawns a pickup at the player's location.");
	PC->ClientMessage("SetLoadedAmmo <LoadedAmmo> - Sets the loaded ammo of the currently equipped weapon.");
	PC->ClientMessage("GiveAmmo [Amount] - Gives ammo for the currently equipped weapon.");
	PC->ClientMessage("ClearEquippedItem - Removes the currently equipped item from the inventory.");
	PC->ClientMessage("GetWeaponStats - Gets the stats of the currently equipped weapon.");
	PC->ClientMessage("ServerExecuteInventoryItem <ItemGuid> - Executes an inventory item by its GUID.");
	PC->ClientMessage("DumpInventory - Dumps inventory stats and items.");
	PC->ClientMessage("-- Player --");
	PC->ClientMessage("SetHealth <Health> / SetMaxHealth <MaxHealth> - Sets the player's health / max health.");
	PC->ClientMessage("SetShield <Shield> / SetMaxShield <MaxShield> - Sets the player's shield / max shield.");
	PC->ClientMessage("SetKillScore <NewScore> - Sets the player's kill score.");
	PC->ClientMessage("TeleportToLocation <X> <Y> <Z> - Teleports the player to a specific location.");
	PC->ClientMessage("TeleportToActor <ActorName> - Teleports the player to a specific actor.");
	PC->ClientMessage("DumpCurrentLocation - Dumps the player's current location and rotation.");
	PC->ClientMessage("-- Pawns / Bots --");
	PC->ClientMessage("DumpAllPawns - Lists every pawn in the world with its index.");
	PC->ClientMessage("PossessPawnByIndex <PawnIndex> - Possesses a pawn by its index (see DumpAllPawns).");
	PC->ClientMessage("PossessPawnByName <PawnName> - Possesses a pawn by name (case-insensitive, substring).");
	PC->ClientMessage("-- World / Actors --");
	PC->ClientMessage("SpawnActor <ActorClassName> [bSetOwnerAsThis] - Spawns an actor at the player.");
	PC->ClientMessage("DumpActorsWithClass <ClassName> - Lists all actors of a class with their locations.");
	PC->ClientMessage("DumpAircrafts - Dumps the aircrafts in the gamestate.");
	PC->ClientMessage("ActivateRifts [Radius] - Force activates every BuildingRift, optionally only within Radius.");
	PC->ClientMessage("DumpEncounters - Dumps the AI director's active encounters and the rifts each one claimed.");
	PC->ClientMessage("StartEncounter [EncounterClassName] - Starts an AI encounter (defaults to the director's BaseEncounterClass).");
	PC->ClientMessage("DestroyTarget - Destroys the actor under the crosshair.");
	PC->ClientMessage("StartEvent - Starts this version's live event.");
	PC->ClientMessage("-- QuickBars --");
	PC->ClientMessage("SpawnQuickBars / DestroyQuickBars / DumpQuickBars - Manage the player's quickbars.");
	PC->ClientMessage("-- Fun --");
	PC->ClientMessage("LootRain [Count] [Radius] [TierGroup] - Rains loot-table drops down around you.");
	PC->ClientMessage("SetGameSpeed [Multiplier] - Sets the game speed. 0.2 = slow motion, 5 = turbo, 1 = normal.");
	PC->ClientMessage("DespawnAllBots - Removes every bot pawn from the world.");
	PC->ClientMessage("TeleportAllToMe - Teleports every other pawn into a ring around you.");
	PC->ClientMessage("SwapPlaces <PawnName> - Swaps locations with a pawn (case-insensitive, substring).");
	PC->ClientMessage("LaunchPawn [PawnName] [ZVelocity] - Yeets a pawn (default: you) into the sky.");
	PC->ClientMessage("SetScale <Multiplier> - Scales your pawn (body, weapon and collision).");
	PC->ClientMessage("ScalePawn <PawnName> <Multiplier> - Scales any pawn. Giant bots!");
	PC->ClientMessage("Goto <PawnName> - Teleports you to a pawn.");
	PC->ClientMessage("DestroyBuildings [Radius] - Destroys every building around you.");
	PC->ClientMessage("EmoteAll - Everyone in the world uses a random emote.");
	PC->ClientMessage("EmoteAllSpecific [EmoteItemDefinitionName] - Everyone in the world uses a specific emote.");
	PC->ClientMessage("EmotePlayerByName <PlayerName> [EmoteItemDefinitionName] - make a player use a specific emote.");
	PC->ClientMessage("TogglePersonalVehicle - Toggle the personal vehicle.");
	PC->ClientMessage("-- AFortAthenaMutator_Heist --");
	PC->ClientMessage("SetSpawnExitCraftTime <TimeLeft> - Sets the SpawnExitCraftTime to the time left.");
	PC->ClientMessage("DumpExitCraftTimer - Logs every gate the native van timer checks.");
	PC->ClientMessage("HeistPhaseStep [Step] - Fires OnGamePhaseStepChanged on the heist mutator (8 = StormShrinking, starts the van timer).");
	PC->ClientMessage("DumpGameState - Dumps the gamestate's phase/storm/playlist state.");
	PC->ClientMessage("UpdateGamePhaseStep - Forces the gamestate to recompute GamePhaseStep and dispatch it to mutators.");
	PC->ClientMessage("SpawnExitCraft [bUseSpawner] [ZOffset] [State] - Spawns a getaway van in front of you (State default 6 = WaitingForPawns).");
}

void UFortCheatManager::GiveItem(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
	int32 Count = Parser.GetArgInt("Count", 1, 1);

	if (ItemDefName.empty())
	{
		PC->ClientMessage("Usage: GiveItem <ItemDefinitionName> [Count]");
		return;
	}

	if (!PC->WorldInventory) {
		PC->ClientMessage("WorldInventory is null!");
		return;
	}

	UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
	if (!ItemObj) {
		PC->ClientMessage("ItemDefinition not found: " + ItemDefName);
		return;
	}

	UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
	if (!ItemDef) {
		PC->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
		return;
	}

	AFortPlayerPawn* Pawn = PC->Pawn->Cast<AFortPlayerPawn>();
	if (!Pawn) {
		PC->ClientMessage("Pawn is null!");
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
		PC->GetDropFinalLocation(),
		-1,
		true,
		true,
		false,
		-1,
		EFortPickupSourceTypeFlag::GetOther(),
		EFortPickupSpawnSource::GetUnset(),
		PC,
		false
	);

	if (Pickup) {
		FVector ZeroVector = { 0, 0, 0 };
		AFortPlayerPawn::ServerHandlePickup(Pawn, Pickup, Pickup->PickupLocationData.FlyTime, ZeroVector, true);
		PC->ClientMessage("Given Item: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
	}
	else {
		PC->ClientMessage("Failed to spawn pickup for item: " + ItemDef->GetName().ToString() + " - try ForceGiveItem.");
	}
}

void UFortCheatManager::ForceGiveItem(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
	int32 Count = Parser.GetArgInt("Count", 1, 1);

	if (ItemDefName.empty())
	{
		PC->ClientMessage("Usage: ForceGiveItem <ItemDefinitionName> [Count]");
		return;
	}

	if (!PC->WorldInventory) {
		PC->ClientMessage("WorldInventory is null!");
		return;
	}

	UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
	if (!ItemObj) {
		PC->ClientMessage("ItemDefinition not found: " + ItemDefName);
		return;
	}

	UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
	if (!ItemDef) {
		PC->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
		return;
	}

	FFortItemEntry* AddedEntry = PC->WorldInventory->AddItem(ItemDef, Count);
	if (AddedEntry) {
		PC->ClientMessage("Force-gave item: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
	}
	else {
		PC->ClientMessage("Failed to add item: " + ItemDef->GetName().ToString());
	}
}

void UFortCheatManager::SpawnPickup(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string ItemDefName = Parser.GetArg("ItemDefinitionName", 0);
	int32 Count = Parser.GetArgInt("Count", 1, 1);

	if (ItemDefName.empty())
	{
		PC->ClientMessage("Usage: SpawnPickup <ItemDefinitionName> [Count]");
		return;
	}

	if (!PC->WorldInventory) {
		PC->ClientMessage("WorldInventory is null!");
		return;
	}

	UObject* ItemObj = Utils::GetObjectFromString(ItemDefName, UFortItemDefinition::StaticClass());
	if (!ItemObj) {
		PC->ClientMessage("ItemDefinition not found: " + ItemDefName);
		return;
	}

	UFortItemDefinition* ItemDef = ItemObj->Cast<UFortItemDefinition>();
	if (!ItemDef) {
		PC->ClientMessage("Object is not a UFortItemDefinition: " + ItemObj->GetName().ToString());
		return;
	}

	AFortPickup* SpawnedPickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		ItemDef,
		Count,
		PC->Pawn->K2_GetActorLocation(),
		PC->GetDropFinalLocation(),
		-1,
		true,
		true,
		true,
		-1,
		EFortPickupSourceTypeFlag::GetPlayer(),
		EFortPickupSpawnSource::GetUnset(),
		PC,
		false
	);

	if (SpawnedPickup) {
		PC->ClientMessage("Spawned pickup: (Item=" + ItemDef->GetName().ToString() + " Count=" + std::to_string(Count) + ")");
	}
	else {
		PC->ClientMessage("Failed to spawn pickup for item: " + ItemDef->GetName().ToString());
	}
}

void UFortCheatManager::SetLoadedAmmo(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("LoadedAmmo"))
	{
		PC->ClientMessage("Usage: SetLoadedAmmo <LoadedAmmo>");
		return;
	}

	int32 LoadedAmmo = Parser.GetArgInt("LoadedAmmo", 0, 30);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}
	AFortWeapon* CurrentWeapon = PC->MyFortPawn->CurrentWeapon;
	if (!CurrentWeapon) {
		PC->ClientMessage("CurrentWeapon is null!");
		return;
	}

	FFortItemEntry* WeaponEntry = PC->FindItemEntry(CurrentWeapon->ItemEntryGuid);
	if (!WeaponEntry) {
		PC->ClientMessage("Weapon entry not found in inventory!");
		return;
	}

	WeaponEntry->LoadedAmmo = LoadedAmmo;
	PC->WorldInventory->Update(WeaponEntry);
	PC->ClientMessage("Set loaded ammo of current weapon to " + std::to_string(LoadedAmmo));
}

void UFortCheatManager::GiveAmmo(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	int32 AmmoAmount = Parser.GetArgInt("Amount", 0, 30);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	AFortWeapon* CurrentWeapon = PC->MyFortPawn->CurrentWeapon;
	if (!CurrentWeapon) {
		PC->ClientMessage("CurrentWeapon is null!");
		return;
	}

	if (!CurrentWeapon->WeaponData) {
		PC->ClientMessage("WeaponData is null!");
		return;
	}

	UFortWorldItemDefinition* ItemDef = CurrentWeapon->WeaponData->Cast<UFortWorldItemDefinition>();
	if (!ItemDef) {
		PC->ClientMessage("WeaponData is not a UFortWorldItemDefinition!");
		return;
	}

	UFortWorldItemDefinition* AmmoItemDef = ItemDef->GetAmmoWorldItemDefinition_BP();
	if (!AmmoItemDef) {
		PC->ClientMessage("AmmoWorldItemDefinition is null!");
		return;
	}

	FFortItemEntry* AmmoEntry = PC->FindItemEntry(AmmoItemDef);
	if (AmmoEntry) {
		AmmoEntry->Count += AmmoAmount;
		PC->WorldInventory->Update(AmmoEntry);
		PC->ClientMessage("Added " + AmmoItemDef->GetName().ToString() + " " + std::to_string(AmmoAmount) + " ammo to existing stack.");
	}
	else {
		AmmoEntry = PC->WorldInventory->AddItem(AmmoItemDef, AmmoAmount);
		if (AmmoEntry) {
			PC->ClientMessage("Added new ammo entry: " + AmmoItemDef->GetName().ToString() + " x" + std::to_string(AmmoAmount));
		}
		else {
			PC->ClientMessage("Failed to add ammo item: " + AmmoItemDef->GetName().ToString());
		}
	}
}

void UFortCheatManager::DumpInventory(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (!PC->WorldInventory) {
		PC->ClientMessage("WorldInventory is null!");
		return;
	}
	PC->ClientMessage("=== Inventory Dump ===");

	PC->ClientMessage("Stats: ");
	PC->ClientMessage("Inventory Capacity: " + std::to_string(PC->WorldInventory->GetInventoryCapacity()));
	PC->ClientMessage("Inventory Used: " + std::to_string(PC->WorldInventory->GetInventoryUsed()));

	PC->ClientMessage("Items: ");
	for (UFortWorldItem* ItemEntry : PC->WorldInventory->Inventory.ItemInstances) {
		std::string ItemName = ItemEntry->ItemEntry.ItemDefinition ? ItemEntry->ItemEntry.ItemDefinition->GetName().ToString() : "Unknown";
		int32 Count = ItemEntry->ItemEntry.Count;
		std::string FormattedGuid = ItemEntry->ItemEntry.ItemGuid.FormatGuid();

		PC->ClientMessage("Item: " + ItemName + ", Count: " + std::to_string(Count) + ", GUID: " + FormattedGuid);
	}

	PC->ClientMessage("=== End of Inventory Dump ===");
}

void UFortCheatManager::SetHealth(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Health"))
	{
		PC->ClientMessage("Usage: SetHealth <Health>");
		return;
	}

	float Health = Parser.GetArgFloat("Health", 0, 100.0f);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	PC->MyFortPawn->SetHealth(Health);

	PC->ClientMessage("Set health to " + std::to_string(Health));
}

void UFortCheatManager::SetShield(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Shield"))
	{
		PC->ClientMessage("Usage: SetShield <Shield>");
		return;
	}

	float Shield = Parser.GetArgFloat("Shield", 0, 100.0f);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	PC->MyFortPawn->SetShield(Shield);

	PC->ClientMessage("Set shield to " + std::to_string(Shield));
}

void UFortCheatManager::SetMaxHealth(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("MaxHealth"))
	{
		PC->ClientMessage("Usage: SetMaxHealth <MaxHealth>");
		return;
	}

	float MaxHealth = Parser.GetArgFloat("MaxHealth", 0, 100.0f);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	PC->MyFortPawn->SetMaxHealth(MaxHealth);

	PC->ClientMessage("Set max health to " + std::to_string(MaxHealth));
}

void UFortCheatManager::SetMaxShield(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("MaxShield"))
	{
		PC->ClientMessage("Usage: SetMaxShield <MaxShield>");
		return;
	}

	float MaxShield = Parser.GetArgFloat("MaxShield", 0, 100.0f);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	PC->MyFortPawn->SetMaxShield(MaxShield);

	PC->ClientMessage("Set max shield to " + std::to_string(MaxShield));
}

void UFortCheatManager::SpawnActor(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string ActorClassName = Parser.GetArg("ActorClassName", 0);
	bool bSetOwnerAsThis = Parser.GetArgBool("bSetOwnerAsThis", 1, false);

	if (ActorClassName.empty())
	{
		PC->ClientMessage("Usage: SpawnActor <ActorClassName> [bSetOwnerAsThis]");
		return;
	}

	FVector Location = PC->Pawn ? PC->Pawn->K2_GetActorLocation() : FVector();
	FRotator Rotation = PC->Pawn ? PC->Pawn->K2_GetActorRotation() : FRotator();

	UObject* ActorClassObj = Utils::GetObjectFromString(ActorClassName, EClassCastFlags::CASTCLASS_UClass);
	if (!ActorClassObj) {
		PC->ClientMessage("Actor class not found: " + ActorClassName);
		return;
	}

	UClass* ActorClass = ActorClassObj->Cast<UClass>();
	if (!ActorClass) {
		PC->ClientMessage("Object is not a UClass: " + ActorClassObj->GetName().ToString());
		return;
	}

	AActor* NewActor = World->SpawnActor(ActorClass, Location, Rotation, bSetOwnerAsThis ? PC : nullptr);
	if (NewActor) {
		PC->ClientMessage("Spawned actor: " + NewActor->GetName().ToString());
	}
	else {
		PC->ClientMessage("Failed to spawn actor of class: " + ActorClass->GetName().ToString());
	}
}

void UFortCheatManager::ClearEquippedItem(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (!PC->WorldInventory) {
		PC->ClientMessage("WorldInventory is null!");
		return;
	}

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	if (!PC->MyFortPawn->CurrentWeapon) {
		PC->ClientMessage("No currently equipped item to clear.");
		return;
	}

	PC->WorldInventory->RemoveItem(PC->MyFortPawn->CurrentWeapon->ItemEntryGuid);
	PC->ClientMessage("Cleared currently equipped item.");
}

void UFortCheatManager::GetWeaponStats(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}
	AFortWeapon* CurrentWeapon = PC->MyFortPawn->CurrentWeapon;
	if (!CurrentWeapon) {
		PC->ClientMessage("CurrentWeapon is null!");
		return;
	}
	UFortWeaponItemDefinition* WeaponDef = CurrentWeapon->WeaponData;
	if (!WeaponDef) {
		PC->ClientMessage("WeaponData is null!");
		return;
	}

	PC->ClientMessage("=== Weapon Stats ===");
	PC->ClientMessage("Weapon Name: " + WeaponDef->GetName().ToString());
	PC->ClientMessage("ClipSize: " + std::to_string(WeaponDef->GetClipSize()));
	PC->ClientMessage("Durability: " + std::to_string(WeaponDef->GetDurability()));
	PC->ClientMessage("=== End of Weapon Stats ===");
}

void UFortCheatManager::DestroyTarget(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UCheatManager::DestroyTarget();
	PC->ClientMessage("Destroyed target.");
}

void UFortCheatManager::DumpActorsWithClass(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string ActorClassName = Parser.GetArg("ClassName", 0);

	if (ActorClassName.empty())
	{
		PC->ClientMessage("Usage: DumpActorsWithClass <ClassName>");
		return;
	}

	UObject* ActorClassObj = Utils::GetObjectFromString(ActorClassName, EClassCastFlags::CASTCLASS_UClass);
	if (!ActorClassObj) {
		PC->ClientMessage("Actor class not found: " + ActorClassName);
		return;
	}

	UClass* ActorClass = ActorClassObj->Cast<UClass>();
	if (!ActorClass) {
		PC->ClientMessage("Object is not a UClass: " + ActorClassObj->GetName().ToString());
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, ActorClass, &FoundActors);

	PC->ClientMessage("Found " + std::to_string(FoundActors.Num()) + " actors of class: " + ActorClass->GetName().ToString());
	for (int32 i = 0; i < FoundActors.Num(); i++) {
		AActor* Actor = FoundActors[i];
		if (!Actor)
			continue;

		FVector ActorLocation = Actor->K2_GetActorLocation();
		PC->ClientMessage("[" + std::to_string(i) + "] " + Actor->GetName().ToString()
			+ " @ X=" + std::to_string(ActorLocation.X) + " Y=" + std::to_string(ActorLocation.Y) + " Z=" + std::to_string(ActorLocation.Z));
	}
}

void UFortCheatManager::TeleportToLocation(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	bool bHasNamedLocation = Parser.HasNamedArg("X") && Parser.HasNamedArg("Y") && Parser.HasNamedArg("Z");
	if (Parser.GetArgCount() < 3 && !bHasNamedLocation)
	{
		PC->ClientMessage("Usage: TeleportToLocation <X> <Y> <Z>");
		return;
	}

	float X = Parser.GetArgFloat("X", 0, 0.0f);
	float Y = Parser.GetArgFloat("Y", 1, 0.0f);
	float Z = Parser.GetArgFloat("Z", 2, 0.0f);

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	FVector NewLocation(X, Y, Z);

	FHitResult HitResult;
	PC->MyFortPawn->K2_SetActorLocation(NewLocation, false, &HitResult, true);
	PC->ClientMessage("Teleported to location: X=" + std::to_string(X) + " Y=" + std::to_string(Y) + " Z=" + std::to_string(Z));
}

void UFortCheatManager::TeleportToActor(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string ActorName = Parser.GetArg("ActorName", 0, "");
	if (ActorName.empty()) {
		PC->ClientMessage("Usage: TeleportToActor <ActorName>");
		return;
	}

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(World, AActor::StaticClass(), &FoundActors);

	std::string LowerActorName = Utils::StringToLower(ActorName);
	AActor* TargetActor = nullptr;
	int32 Matches = 0;

	for (int32 i = 0; i < FoundActors.Num(); i++) {
		AActor* Actor = FoundActors[i];
		if (!Actor || Actor == PC->MyFortPawn)
			continue;

		std::string LowerName = Utils::StringToLower(Actor->GetName().ToString());
		if (LowerName == LowerActorName) {
			TargetActor = Actor;
			Matches = 1;
			break;
		}

		if (LowerName.find(LowerActorName) != std::string::npos) {
			if (!TargetActor)
				TargetActor = Actor;
			Matches++;
		}
	}

	if (!TargetActor) {
		PC->ClientMessage("No actor found matching: " + ActorName);
		return;
	}

	FVector TargetLocation = TargetActor->K2_GetActorLocation();
	TargetLocation.Z += 100.f;

	FHitResult HitResult;
	PC->MyFortPawn->K2_SetActorLocation(TargetLocation, false, &HitResult, true);

	std::string Message = "Teleported to " + TargetActor->GetName().ToString()
		+ " @ X=" + std::to_string(TargetLocation.X) + " Y=" + std::to_string(TargetLocation.Y) + " Z=" + std::to_string(TargetLocation.Z);
	if (Matches > 1) {
		Message += " (" + std::to_string(Matches) + " matches, teleported to first)";
	}
	PC->ClientMessage(Message);
}

void UFortCheatManager::StartEvent(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Events::StartEvent()) {
		PC->ClientMessage("Event started!");
	}
	else {
		PC->ClientMessage("Failed to start event, check the log.");
	}
}

void UFortCheatManager::DumpCurrentLocation(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	FVector CurrentLocation = PC->MyFortPawn->K2_GetActorLocation();
	FRotator CurrentRotation = PC->MyFortPawn->K2_GetActorRotation();

	PC->ClientMessage("Location: X=" + std::to_string(CurrentLocation.X) + " Y=" + std::to_string(CurrentLocation.Y) + " Z=" + std::to_string(CurrentLocation.Z));
	PC->ClientMessage("Rotation: Pitch=" + std::to_string(CurrentRotation.Pitch) + " Yaw=" + std::to_string(CurrentRotation.Yaw));
}

void UFortCheatManager::SpawnQuickBars(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	PC->SpawnQuickBars();
	PC->SetupQuickBars();
	PC->ClientMessage("Spawned QuickBars.");
}

void UFortCheatManager::DestroyQuickBars(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (PC->QuickBars) {
		PC->QuickBars->K2_DestroyActor();
		PC->QuickBars = nullptr;
		PC->ForceNetUpdate();
		PC->ClientMessage("Destroyed QuickBars.");
	}
	else if (PC->ClientQuickBars) {
		PC->ClientQuickBars->K2_DestroyActor();
		PC->ClientQuickBars = nullptr;
		PC->ForceNetUpdate();
		PC->ClientMessage("Destroyed ClientQuickBars.");
	}
	else {
		PC->ClientMessage("No QuickBars to destroy.");
	}
}

void UFortCheatManager::DumpQuickBars(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (PC->QuickBars) {
		PC->ClientMessage("QuickBars: " + PC->QuickBars->GetName().ToString() + " @ 0x" + std::format("{:X}", (uintptr_t)PC->QuickBars));
	}
	else if (PC->ClientQuickBars) {
		PC->ClientMessage("ClientQuickBars: " + PC->ClientQuickBars->GetName().ToString() + " @ 0x" + std::format("{:X}", (uintptr_t)PC->ClientQuickBars));
	}
	else {
		PC->ClientMessage("No QuickBars to dump.");
	}
}

void UFortCheatManager::ServerExecuteInventoryItem(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	std::string GuidA = Parser.GetArg("ItemGuid", 0);

	if (GuidA.empty())
	{
		PC->ClientMessage("Usage: ServerExecuteInventoryItem <ItemGuid>");
		return;
	}

	FGuid ItemGuid = FGuid::ParseGUID(GuidA);

	if (!PC->FindItemInstance(ItemGuid)) {
		PC->ClientMessage("No inventory item with GUID: " + GuidA + " (see DumpInventory)");
		return;
	}

	PC->ServerExecuteInventoryItem(PC, ItemGuid);
	PC->ClientMessage("Executed inventory item: " + GuidA);
}

void UFortCheatManager::PossessPawnByIndex(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("PawnIndex"))
	{
		PC->ClientMessage("Usage: PossessPawnByIndex <PawnIndex>");
		return;
	}

	int32 PawnIndex = Parser.GetArgInt("PawnIndex", 0, 0);

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	if (PawnIndex < 0 || PawnIndex >= Pawns.Num()) {
		PC->ClientMessage("Invalid PawnIndex. Must be between 0 and " + std::to_string(Pawns.Num() - 1));
		return;
	}

	APawn* TargetPawn = (APawn*)Pawns[PawnIndex];
	if (!TargetPawn) {
		PC->ClientMessage("TargetPawn is null!");
		return;
	}

	if (TargetPawn == PC->K2_GetPawn()) {
		PC->ClientMessage("Already possessing that pawn.");
		return;
	}

	if (TargetPawn->Controller && TargetPawn->Controller != PC) {
		PC->ClientMessage("Warning: pawn is currently controlled by " + TargetPawn->Controller->GetName().ToString() + " -- taking it over.");
	}

	APawn* PreviousPawn = PC->K2_GetPawn();

	PC->PossessVFT(TargetPawn);

	PC->ForceNetUpdate();
	TargetPawn->ForceNetUpdate();

	PC->ClientMessage("Possessed pawn: " + TargetPawn->GetName().ToString() + " (" + TargetPawn->GetClass()->GetName().ToString() + ") at index " + std::to_string(PawnIndex));
	if (PreviousPawn) {
		PC->ClientMessage("Left behind: " + PreviousPawn->GetName().ToString());
		if (PreviousPawn->Controller) {
			Log("PossessPawnByIndex: previous pawn " + PreviousPawn->GetName().ToString() + " still has a Controller (" + PreviousPawn->Controller->GetName().ToString() + ") after possession!");
			PC->ClientMessage("Warning: previous pawn did not fully detach.");
		}
	}
}

void UFortCheatManager::PossessPawnByName(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PawnName = Parser.GetArg("PawnName", 0);

	if (PawnName.empty())
	{
		PC->ClientMessage("Usage: PossessPawnByName <PawnName> (case-insensitive, matches substrings)");
		return;
	}

	std::string PawnNameLower = Utils::StringToLower(PawnName);

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	APawn* ExactMatch = nullptr;
	TArray<APawn*> PartialMatches;

	for (AActor* Actor : Pawns) {
		if (!Actor)
			continue;

		std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());

		if (ActorNameLower == PawnNameLower) {
			ExactMatch = (APawn*)Actor;
			break;
		}

		if (ActorNameLower.find(PawnNameLower) != std::string::npos) {
			PartialMatches.Add((APawn*)Actor);
		}
	}

	APawn* TargetPawn = nullptr;

	if (ExactMatch) {
		TargetPawn = ExactMatch;
	}
	else if (PartialMatches.Num() == 1) {
		TargetPawn = PartialMatches[0];
	}
	else if (PartialMatches.Num() > 1) {
		PC->ClientMessage("'" + PawnName + "' is ambiguous, matches " + std::to_string(PartialMatches.Num()) + " pawns:");
		for (int32 i = 0; i < PartialMatches.Num(); i++) {
			PC->ClientMessage("  " + PartialMatches[i]->GetName().ToString());
		}
		PC->ClientMessage("Be more specific.");
		return;
	}
	else {
		PC->ClientMessage("Pawn with name '" + PawnName + "' not found.");
		return;
	}

	if (TargetPawn == PC->K2_GetPawn()) {
		PC->ClientMessage("Already possessing that pawn.");
		return;
	}

	if (TargetPawn->Controller && TargetPawn->Controller != PC) {
		PC->ClientMessage("Warning: pawn is currently controlled by " + TargetPawn->Controller->GetName().ToString() + " -- taking it over.");
	}

	APawn* PreviousPawn = PC->K2_GetPawn();

	PC->PossessVFT(TargetPawn);

	PC->ForceNetUpdate();
	TargetPawn->ForceNetUpdate();

	PC->ClientMessage("Possessed pawn: " + TargetPawn->GetName().ToString() + " (" + TargetPawn->GetClass()->GetName().ToString() + ")");
	if (PreviousPawn) {
		PC->ClientMessage("Left behind: " + PreviousPawn->GetName().ToString());
		if (PreviousPawn->Controller) {
			Log("PossessPawnByName: previous pawn " + PreviousPawn->GetName().ToString() + " still has a Controller (" + PreviousPawn->Controller->GetName().ToString() + ") after possession!");
			PC->ClientMessage("Warning: previous pawn did not fully detach.");
		}
	}
}

void UFortCheatManager::DumpAllPawns(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	PC->ClientMessage("Found " + std::to_string(Pawns.Num()) + " pawns in the world.");

	for (int32 i = 0; i < Pawns.Num(); ++i) {
		APawn* IndexedPawn = (APawn*)Pawns[i];
		if (!IndexedPawn)
			continue;

		std::string Line = "[" + std::to_string(i) + "] " + IndexedPawn->GetName().ToString() + " (" + IndexedPawn->GetClass()->GetName().ToString() + ")";
		if (IndexedPawn->Controller)
			Line += " - controlled by " + IndexedPawn->Controller->GetName().ToString();
		PC->ClientMessage(Line);
	}
}

void UFortCheatManager::SetKillScore(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("NewScore"))
	{
		PC->ClientMessage("Usage: SetKillScore <NewScore>");
		return;
	}

	int32 NewScore = Parser.GetArgInt("NewScore", 0, 0);

	AFortPlayerStateAthena* PlayerState = PC->PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerState) {
		PC->ClientMessage("PlayerState is null or not AFortPlayerStateAthena!");
		return;
	}

	PlayerState->SetKillScore(NewScore);
	PC->ClientMessage("Set kill score to " + std::to_string(NewScore));
}

void UFortCheatManager::LootRain(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameMode* GameMode = World->AuthorityGameMode->Cast<AFortGameMode>();
	if (!GameMode)
		return;

	if (!PC->Pawn) {
		PC->ClientMessage("Pawn is null!");
		return;
	}

	int32 Count = Parser.GetArgInt("Count", 0, 20);
	float Radius = Parser.GetArgFloat("Radius", 1, 600.0f);
	if (Count < 1) Count = 1;
	if (Count > 100) Count = 100;
	if (Radius < 100.0f) Radius = 100.0f;

	FName TierGroup;
	std::string TierGroupArg = Parser.GetArg("TierGroup", 2);
	if (!TierGroupArg.empty()) {
		TierGroup = UKismetStringLibrary::Conv_StringToName(TierGroupArg);
	}
	else if (GameMode->Cast<AFortGameModeAthena>()) {
		static FName Loot_AthenaTreasure = UKismetStringLibrary::Conv_StringToName("Loot_AthenaTreasure");
		TierGroup = Loot_AthenaTreasure;
	}
	else {
		static FName Loot_Treasure = UKismetStringLibrary::Conv_StringToName("Loot_Treasure");
		TierGroup = Loot_Treasure;
	}

	FVector Center = PC->Pawn->K2_GetActorLocation();
	int32 Spawned = 0;

	for (int32 Roll = 0; Spawned < Count && Roll < Count * 4; Roll++) {
		TArray<FFortItemEntry> LootDrops;
		if (!UFortKismetLibrary::PickLootDrops(PC, &LootDrops, TierGroup, 0, -1) || LootDrops.Num() == 0)
			break;

		for (int32 i = 0; i < LootDrops.Num() && Spawned < Count; i++) {
			FFortItemEntry& Entry = LootDrops.GetWithSize(i, FFortItemEntry::GetSize());
			if (!Entry.ItemDefinition)
				continue;

			const float Angle = ((float)rand() / (float)RAND_MAX) * 6.2831853f;
			const float Dist = ((float)rand() / (float)RAND_MAX) * Radius;

			FVector DropLoc = Center;
			DropLoc.X += cosf(Angle) * Dist;
			DropLoc.Y += sinf(Angle) * Dist;

			FVector AirLoc = DropLoc;
			AirLoc.Z += 400.0f + ((float)rand() / (float)RAND_MAX) * 400.0f;

			AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
				World,
				Entry.ItemDefinition,
				Entry.Count,
				AirLoc,
				DropLoc,
				-1,
				true,
				true,
				true,
				-1,
				EFortPickupSourceTypeFlag::GetOther(),
				EFortPickupSpawnSource::GetUnset(),
				PC,
				false
			);

			if (!Pickup)
				continue;

			UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
			if (WeaponDef) {
				int32 Level = Pickup->PrimaryPickupItemEntry.Level;
				Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
				Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
				Pickup->PrimaryPickupItemEntry.bIsDirty = true;
				Pickup->PrimaryPickupItemEntry.ReplicationKey++;
				Pickup->OnRep_PrimaryPickupItemEntry();
			}

			Spawned++;
		}
	}

	if (Spawned > 0)
		PC->ClientMessage("Rained " + std::to_string(Spawned) + " items from '" + TierGroup.ToString().ToString() + "'.");
	else
		PC->ClientMessage("PickLootDrops found nothing for tier group '" + TierGroup.ToString().ToString() + "'.");
}

void UFortCheatManager::SetGameSpeed(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	float Speed = Parser.GetArgFloat("Multiplier", 0, 1.0f);
	if (Speed < 0.05f) Speed = 0.05f;
	if (Speed > 10.0f) Speed = 10.0f;

	AWorldSettings* WorldSettings = World->GetWorldSettings();
	if (!WorldSettings) {
		PC->ClientMessage("WorldSettings is null!");
		return;
	}

	WorldSettings->TimeDilation = Speed;
	WorldSettings->ForceNetUpdate();

	if (Speed == 1.0f)
		PC->ClientMessage("Game speed back to normal.");
	else
		PC->ClientMessage("Game speed set to x" + std::to_string(Speed) + ". Use SetGameSpeed 1 to reset.");
}

void UFortCheatManager::DespawnAllBots(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	int32 Removed = 0;

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	for (AActor* Actor : Pawns) {
		APawn* TargetPawn = (APawn*)Actor;
		if (!TargetPawn || TargetPawn == PC->K2_GetPawn())
			continue;

		if (!TargetPawn->IsBotControlled())
			continue;

		TargetPawn->K2_DestroyActor();
		Removed++;
	}

	PC->ClientMessage("Despawned " + std::to_string(Removed) + " bot pawns.");
}

void UFortCheatManager::TeleportAllToMe(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	FVector Center = PC->MyFortPawn->K2_GetActorLocation();
	int32 Moved = 0;
	for (AActor* Actor : Pawns) {
		APawn* TargetPawn = (APawn*)Actor;
		if (!TargetPawn || TargetPawn == PC->MyFortPawn)
			continue;

		const float Angle = (Moved % 8) * 0.785398f;
		const float Ring = 300.0f + 150.0f * (Moved / 8);

		FVector NewLocation = Center;
		NewLocation.X += cosf(Angle) * Ring;
		NewLocation.Y += sinf(Angle) * Ring;
		NewLocation.Z += 50.0f;

		FHitResult HitResult;
		TargetPawn->K2_SetActorLocation(NewLocation, false, &HitResult, true);
		Moved++;
	}

	PC->ClientMessage("Teleported " + std::to_string(Moved) + " pawns to you.");
}

void UFortCheatManager::SwapPlaces(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PawnName = Parser.GetArg("PawnName", 0);

	if (PawnName.empty())
	{
		PC->ClientMessage("Usage: SwapPlaces <PawnName> (case-insensitive, matches substrings)");
		return;
	}

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	std::string PawnNameLower = Utils::StringToLower(PawnName);

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	APawn* ExactMatch = nullptr;
	TArray<APawn*> PartialMatches;

	for (AActor* Actor : Pawns) {
		if (!Actor || Actor == PC->MyFortPawn)
			continue;

		std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());

		if (ActorNameLower == PawnNameLower) {
			ExactMatch = (APawn*)Actor;
			break;
		}

		if (ActorNameLower.find(PawnNameLower) != std::string::npos) {
			PartialMatches.Add((APawn*)Actor);
		}
	}

	APawn* TargetPawn = nullptr;

	if (ExactMatch) {
		TargetPawn = ExactMatch;
	}
	else if (PartialMatches.Num() == 1) {
		TargetPawn = PartialMatches[0];
	}
	else if (PartialMatches.Num() > 1) {
		PC->ClientMessage("'" + PawnName + "' is ambiguous, matches " + std::to_string(PartialMatches.Num()) + " pawns:");
		for (int32 i = 0; i < PartialMatches.Num(); i++) {
			PC->ClientMessage("  " + PartialMatches[i]->GetName().ToString());
		}
		PC->ClientMessage("Be more specific.");
		return;
	}
	else {
		PC->ClientMessage("Pawn with name '" + PawnName + "' not found.");
		return;
	}

	FVector MyLocation = PC->MyFortPawn->K2_GetActorLocation();
	FVector TheirLocation = TargetPawn->K2_GetActorLocation();

	FHitResult HitResult;
	PC->MyFortPawn->K2_SetActorLocation(TheirLocation, false, &HitResult, true);
	TargetPawn->K2_SetActorLocation(MyLocation, false, &HitResult, true);

	PC->ClientMessage("Swapped places with " + TargetPawn->GetName().ToString());
}

void UFortCheatManager::LaunchPawn(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PawnName;
	float ZVelocity = 3000.0f;

	if (Parser.GetArgCount() >= 1) {
		std::string FirstArg = Parser.GetArg(0);

		char* End = nullptr;
		float ParsedVelocity = strtof(FirstArg.c_str(), &End);
		if (End != FirstArg.c_str() && *End == '\0') {
			ZVelocity = ParsedVelocity;
		}
		else {
			PawnName = FirstArg;
			ZVelocity = Parser.GetArgFloat(1, 3000.0f);
		}
	}

	if (Parser.HasNamedArg("PawnName"))
		PawnName = Parser.GetNamedArg("PawnName");
	if (Parser.HasNamedArg("ZVelocity"))
		ZVelocity = Parser.GetArgFloat("ZVelocity", 1, 3000.0f);

	APawn* TargetPawn = PC->K2_GetPawn();

	if (!PawnName.empty()) {
		std::string PawnNameLower = Utils::StringToLower(PawnName);

		TArray<AActor*> Pawns;
		UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

		TargetPawn = nullptr;
		for (AActor* Actor : Pawns) {
			if (!Actor)
				continue;

			std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
			if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
				TargetPawn = (APawn*)Actor;
				break;
			}
		}

		if (!TargetPawn) {
			PC->ClientMessage("Pawn with name '" + PawnName + "' not found.");
			return;
		}
	}

	if (!TargetPawn) {
		PC->ClientMessage("No pawn to launch!");
		return;
	}

	ACharacter* TargetCharacter = TargetPawn->Cast<ACharacter>();
	if (!TargetCharacter) {
		PC->ClientMessage(TargetPawn->GetName().ToString() + " is not a Character, cannot launch it.");
		return;
	}

	TargetCharacter->LaunchCharacter(FVector(0.0f, 0.0f, ZVelocity), false, true);
	PC->ClientMessage("Launched " + TargetCharacter->GetName().ToString() + " with ZVelocity " + std::to_string(ZVelocity) + ".");
}

void UFortCheatManager::SetScale(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	if (Parser.GetArgCount() < 1 && !Parser.HasNamedArg("Multiplier"))
	{
		PC->ClientMessage("Usage: SetScale <Multiplier>");
		return;
	}

	float Scale = Parser.GetArgFloat("Multiplier", 0, 1.0f);
	if (Scale < 0.1f) Scale = 0.1f;
	if (Scale > 10.0f) Scale = 10.0f;

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	UCapsuleComponent* Capsule = PC->MyFortPawn->GetCapsuleComponent();
	if (!Capsule) {
		PC->ClientMessage("Pawn has no capsule component!");
		return;
	}

	Capsule->SetIsReplicated(true);
	PC->MyFortPawn->SetActorScale3D(FVector(Scale, Scale, Scale));
	PC->MyFortPawn->ForceNetUpdate();

	PC->ClientMessage("Set scale to x" + std::to_string(Scale) + ".");
}

void UFortCheatManager::ScalePawn(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PawnName = Parser.GetArg("PawnName", 0);
	bool bHasMultiplier = Parser.GetArgCount() >= 2 || Parser.HasNamedArg("Multiplier");

	if (PawnName.empty() || !bHasMultiplier)
	{
		PC->ClientMessage("Usage: ScalePawn <PawnName> <Multiplier> (case-insensitive, matches substrings)");
		return;
	}

	float Scale = Parser.GetArgFloat("Multiplier", 1, 1.0f);
	if (Scale < 0.1f) Scale = 0.1f;
	if (Scale > 10.0f) Scale = 10.0f;

	std::string PawnNameLower = Utils::StringToLower(PawnName);

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	APawn* TargetPawn = nullptr;
	for (AActor* Actor : Pawns) {
		if (!Actor)
			continue;

		std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
		if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
			TargetPawn = (APawn*)Actor;
			break;
		}
	}

	if (!TargetPawn) {
		PC->ClientMessage("Pawn with name '" + PawnName + "' not found.");
		return;
	}

	ACharacter* TargetCharacter = TargetPawn->Cast<ACharacter>();
	if (!TargetCharacter) {
		PC->ClientMessage(TargetPawn->GetName().ToString() + " is not a Character, cannot scale it.");
		return;
	}

	UCapsuleComponent* Capsule = TargetCharacter->GetCapsuleComponent();
	if (Capsule)
		Capsule->SetIsReplicated(true);

	TargetCharacter->SetActorScale3D(FVector(Scale, Scale, Scale));
	TargetCharacter->ForceNetUpdate();

	PC->ClientMessage("Scaled " + TargetCharacter->GetName().ToString() + " to x" + std::to_string(Scale));
}

void UFortCheatManager::Goto(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PawnName = Parser.GetArg("PawnName", 0);

	if (PawnName.empty())
	{
		PC->ClientMessage("Usage: Goto <PawnName> (case-insensitive, matches substrings)");
		return;
	}

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	std::string PawnNameLower = Utils::StringToLower(PawnName);

	TArray<AActor*> Pawns;
	UGameplayStatics::GetAllActorsOfClass(World, APawn::StaticClass(), &Pawns);

	APawn* TargetPawn = nullptr;
	for (AActor* Actor : Pawns) {
		if (!Actor)
			continue;

		std::string ActorNameLower = Utils::StringToLower(Actor->GetName().ToString());
		if (ActorNameLower == PawnNameLower || ActorNameLower.find(PawnNameLower) != std::string::npos) {
			TargetPawn = (APawn*)Actor;
			break;
		}
	}

	if (!TargetPawn) {
		PC->ClientMessage("Pawn with name '" + PawnName + "' not found.");
		return;
	}

	if (TargetPawn == PC->MyFortPawn) {
		PC->ClientMessage("That's you.");
		return;
	}

	FVector TargetLocation = TargetPawn->K2_GetActorLocation();
	TargetLocation.X += 150.0f;
	TargetLocation.Z += 50.0f;

	FHitResult HitResult;
	PC->MyFortPawn->K2_SetActorLocation(TargetLocation, false, &HitResult, true);
	PC->ClientMessage("Teleported to " + TargetPawn->GetName().ToString());
}

void UFortCheatManager::DestroyBuildings(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	if (!PC->MyFortPawn) {
		PC->ClientMessage("MyFortPawn is null!");
		return;
	}

	float Radius = Parser.GetArgFloat("Radius", 0, 2000.0f);
	if (Radius < 500.0f) Radius = 500.0f;
	if (Radius > 10000.0f) Radius = 10000.0f;

	TArray<AActor*> Buildings;
	UGameplayStatics::GetAllActorsOfClass(World, ABuildingActor::StaticClass(), &Buildings);

	int32 Destroyed = 0;
	for (AActor* Building : Buildings) {
		if (!Building)
			continue;

		if (PC->MyFortPawn->GetDistanceTo(Building) > Radius)
			continue;

		Building->K2_DestroyActor();
		Destroyed++;
	}

	PC->ClientMessage("Destroyed " + std::to_string(Destroyed) + " buildings within " + std::to_string((int32)Radius) + " units.");
}

void UFortCheatManager::EmoteAll(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
	if (Emotes.Num() == 0) {
		PC->ClientMessage("No emotes are loaded!");	
		return;
	}

	TArray<AActor*> Controllers;
	UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

	int32 Dancing = 0;
	for (AActor* Actor : Controllers) {
		AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
		if (!Controller)
			continue;

		AFortPlayerController::ServerPlayEmoteItem(Controller, (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()], 0.0f);
		Dancing++;
	}

	PC->ClientMessage(std::to_string(Dancing) + " pawns are emoting.");
}

void UFortCheatManager::EmoteAllSpecific(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string EmoteItemDefinitionName = Parser.GetArg("EmoteItemDefinitionName", 0);

	UFortMontageItemDefinitionBase* ChosenEmote = nullptr;
	if (!EmoteItemDefinitionName.empty()) {
		ChosenEmote = (UFortMontageItemDefinitionBase*)Utils::GetObjectFromString(EmoteItemDefinitionName, UFortMontageItemDefinitionBase::StaticClass());
		if (!ChosenEmote) {
			PC->ClientMessage("EmoteItemDefinition not found: " + EmoteItemDefinitionName);
			return;
		}
	}
	else {
		TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
		if (Emotes.Num() == 0) {
			PC->ClientMessage("No emotes are loaded!");
			return;
		}

		ChosenEmote = (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()];
	}

	TArray<AActor*> Controllers;
	UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

	int32 Dancing = 0;
	for (AActor* Actor : Controllers) {
		AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
		if (!Controller)
			continue;

		AFortPlayerController::ServerPlayEmoteItem(Controller, ChosenEmote, 0.0f);
		Dancing++;
	}

	PC->ClientMessage(std::to_string(Dancing) + " pawns are emoting.");
}

void UFortCheatManager::EmotePlayerByName(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	std::string PlayerName = Parser.GetArg("PlayerName", 0);

	if (PlayerName.empty())
	{
		PC->ClientMessage("Usage: EmotePlayerByName <PlayerName> [EmoteItemDefinitionName]");
		return;
	}

	std::string EmoteItemDefinitionName = Parser.GetArg("EmoteItemDefinitionName", 0);

	UFortMontageItemDefinitionBase* ChosenEmote = nullptr;
	if (!EmoteItemDefinitionName.empty()) {
		ChosenEmote = (UFortMontageItemDefinitionBase*)Utils::GetObjectFromString(EmoteItemDefinitionName, UFortMontageItemDefinitionBase::StaticClass());
		if (!ChosenEmote) {
			PC->ClientMessage("EmoteItemDefinition not found: " + EmoteItemDefinitionName);
			return;
		}
	}
	else {
		TArray<UObject*> Emotes = FUObjectArray::GetObjectsOfClass(UFortMontageItemDefinitionBase::StaticClass());
		if (Emotes.Num() == 0) {
			PC->ClientMessage("No emotes are loaded!");
			return;
		}

		ChosenEmote = (UFortMontageItemDefinitionBase*)Emotes[rand() % Emotes.Num()];
	}

	TArray<AActor*> Controllers;
	UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerController::StaticClass(), &Controllers);

	for (AActor* Actor : Controllers) {
		AFortPlayerController* Controller = Actor ? Actor->Cast<AFortPlayerController>() : nullptr;
		if (!Controller)
			continue;

		AFortPlayerState* PlayerState = Controller->PlayerState->Cast<AFortPlayerState>();
		if (!PlayerState || PlayerState->GetPlayerName() != PlayerName)
			continue;

		AFortPlayerController::ServerPlayEmoteItem(Controller, ChosenEmote, 0.0f);
		PC->ClientMessage("Player: " + PlayerName + " is emoting!");
		return;
	}
}

void UFortCheatManager::TogglePersonalVehicle(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	bool bOn = !PC->IsPersonalVehicleActive();
	PC->TogglePersonalVehicle(bOn);
	PC->ClientMessage("Toggled Personal Vehicle!");
}

void UFortCheatManager::DumpAircrafts(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	TArray<AFortAthenaAircraft*>& Aircrafts = GameState->Aircrafts;
	if (Aircrafts.IsEmpty()) {
		PC->ClientMessage("Aircrafts array is empty!");
		return;
	}

	PC->ClientMessage("===== Start Aircrafts Dump =====");
	for (AFortAthenaAircraft* Aircraft : Aircrafts) {
		PC->ClientMessage("Aircraft: " + Aircraft->GetName().ToString());
	}
	PC->ClientMessage("===== End Aircrafts Dump =====");
}

void UFortCheatManager::SetSpawnExitCraftTime(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	float TimeLeft = Parser.GetArgFloat("TimeLeft", 0, -1.0f);

	if (TimeLeft < 0.0f) {
		PC->ClientMessage("Usage: SetSpawnExitCraftTime <TimeLeft>");
		return;
	}

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	AFortAthenaMutator_Heist* HeistMutator = nullptr;
	for (AFortAthenaMutator* Mutator : GameState->GameplayMutators) {
		if (Mutator && (HeistMutator = Mutator->Cast<AFortAthenaMutator_Heist>())) {
			break;
		}
	}

	if (!HeistMutator) {
		PC->ClientMessage("No AFortAthenaMutator_Heist in GameplayMutators! Are you on a Getaway playlist?");
		return;
	}

	float WorldTime = UGameplayStatics::GetTimeSeconds(World);
	float OldTimeLeft = HeistMutator->SpawnExitCraftTime - WorldTime;
	HeistMutator->SpawnExitCraftTime = WorldTime + TimeLeft;

	HeistMutator->ForceNetUpdate();

	PC->ClientMessage(std::format("Set SpawnExitCraftTime: exit craft in {:.1f}s (was {:.1f}s)!", TimeLeft, OldTimeLeft));
}

void UFortCheatManager::DumpExitCraftTimer(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	AFortAthenaMutator_Heist* HeistMutator = nullptr;
	for (AFortAthenaMutator* Mutator : GameState->GameplayMutators) {
		if (Mutator && (HeistMutator = Mutator->Cast<AFortAthenaMutator_Heist>())) {
			break;
		}
	}

	if (!HeistMutator) {
		PC->ClientMessage("No AFortAthenaMutator_Heist in GameplayMutators! Are you on a Getaway playlist?");
		return;
	}

	if (Parser.IsCommand("HeistPhaseStep")) {
		uint8 Step = (uint8)Parser.GetArgInt("Step", 0, 8);

		HeistMutator->OnGamePhaseStepChanged(Step);
		PC->ClientMessage("Fired OnGamePhaseStepChanged(" + std::to_string(Step) + ") on " + HeistMutator->GetName().ToString());
	}

	PC->ClientMessage("===== Heist ExitCraft Timer State =====");
	PC->ClientMessage("Role: " + std::to_string(HeistMutator->Role));
	PC->ClientMessage("CachedGameMode: " + std::string(HeistMutator->CachedGameMode ? "set" : "NULL"));
	PC->ClientMessage("CachedGameState: " + std::string(HeistMutator->CachedGameState ? "set" : "NULL"));
	PC->ClientMessage("SpawnExitCraftTime: " + std::to_string(HeistMutator->SpawnExitCraftTime));
	PC->ClientMessage("CurrExitCraftIndexToSpawn: " + std::to_string(HeistMutator->CurrExitCraftIndexToSpawn));
	PC->ClientMessage("HeistExitCraftSpawnData.Num: " + std::to_string(HeistMutator->HeistExitCraftSpawnData.Num()));
	PC->ClientMessage("RemainingExitCraftSpawnIndexes.Num: " + std::to_string(HeistMutator->RemainingExitCraftSpawnIndexes.Num()));
	PC->ClientMessage("SpawnedExitCraftList.Num: " + std::to_string(HeistMutator->SpawnedExitCraftList.Num()));
	PC->ClientMessage("NumUnspawned: " + std::to_string(HeistMutator->NumUnspawnedExitCrafts) + " NumSpawned: " + std::to_string(HeistMutator->NumSpawnedExitCrafts) + " NumDeparted: " + std::to_string(HeistMutator->NumDepartedExitCrafts));

	if (GameState->_HasSafeZonePhase()) {
		PC->ClientMessage("GameState SafeZonePhase: " + std::to_string(GameState->SafeZonePhase));
	}
	if (GameState->_HasGamePhaseStep()) {
		PC->ClientMessage("GameState GamePhaseStep: " + std::to_string(GameState->GamePhaseStep));
	}

	if (HeistMutator->CurrExitCraftIndexToSpawn >= 0 && HeistMutator->CurrExitCraftIndexToSpawn < HeistMutator->HeistExitCraftSpawnData.Num()) {
		FHeistExitCraftSpawnData& SpawnData = HeistMutator->HeistExitCraftSpawnData.GetWithSize(HeistMutator->CurrExitCraftIndexToSpawn, FHeistExitCraftSpawnData::GetSize());

		PC->ClientMessage("Next van WhenToSpawn phase: " + std::to_string((int32)SpawnData.SafeZonePhaseWhenToSpawn.Evaluate(0))
			+ " WhereToSpawn index: " + std::to_string((int32)SpawnData.SafeZonePhaseWhereToSpawn.Evaluate(0)));
	}

	PC->ClientMessage("===== End Heist ExitCraft Timer State =====");
}

void UFortCheatManager::UpdateGamePhaseStep(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	uint8 Before = GameState->_HasGamePhaseStep() ? GameState->GamePhaseStep : 255;
	GameState->UpdateGamePhaseStep();
	uint8 After = GameState->_HasGamePhaseStep() ? GameState->GamePhaseStep : 255;

	PC->ClientMessage("UpdateGamePhaseStep: GamePhaseStep " + std::to_string(Before) + " -> " + std::to_string(After));
}

void UFortCheatManager::DumpGameState(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	PC->ClientMessage("===== GameState Dump =====");
	if (GameState->_HasGamePhase()) {
		PC->ClientMessage("GamePhase: " + std::to_string(GameState->GamePhase));
	}
	if (GameState->_HasGamePhaseStep()) {
		PC->ClientMessage("GamePhaseStep: " + std::to_string(GameState->GamePhaseStep));
	}
	if (GameState->_HasSafeZonePhase()) {
		PC->ClientMessage("SafeZonePhase: " + std::to_string(GameState->SafeZonePhase));
	}
	if (GameState->_HasCachedSafeZoneStartUp()) {
		PC->ClientMessage("CachedSafeZoneStartUp: " + std::to_string(GameState->CachedSafeZoneStartUp));
	}
	PC->ClientMessage("SafeZoneIndicator: " + std::string(GameState->SafeZoneIndicator ? "set" : "NULL"));
	if (GameState->SafeZoneIndicator) {
		FVector Center = GameState->SafeZoneIndicator->GetSafeZoneCenter();
		PC->ClientMessage(std::format("SafeZone Center: {:.0f}, {:.0f}, {:.0f} Radius: {:.0f}", Center.X, Center.Y, Center.Z, GameState->SafeZoneIndicator->GetSafeZoneRadius()));
	}
	PC->ClientMessage("MapInfo: " + std::string(GameState->MapInfo ? "set" : "NULL"));
	if (GameState->MapInfo) {
		PC->ClientMessage("MapInfo SafeZoneDefinitions.Num: " + std::to_string(GameState->MapInfo->SafeZoneDefinitions.Num()));
	}
	if (GameState->_HasCurrentPlaylistId()) {
		PC->ClientMessage("CurrentPlaylistId: " + std::to_string(GameState->CurrentPlaylistId));
	}
	if (GameState->_HasbPlaylistDataIsLoaded()) {
		PC->ClientMessage("bPlaylistDataIsLoaded: " + std::string(GameState->bPlaylistDataIsLoaded ? "true" : "false"));
	}
	if (GameState->_HasTotalPlayers()) {
		PC->ClientMessage("TotalPlayers: " + std::to_string(GameState->TotalPlayers) + " PlayersLeft: " + std::to_string(GameState->PlayersLeft) + " TeamsLeft: " + std::to_string(GameState->TeamsLeft));
	}
	if (GameState->_HasAirCraftBehavior()) {
		PC->ClientMessage("AirCraftBehavior: " + std::to_string(GameState->AirCraftBehavior));
	}
	if (GameState->_HasbStormReachedFinalPosition()) {
		PC->ClientMessage("bStormReachedFinalPosition: " + std::string(GameState->bStormReachedFinalPosition ? "true" : "false"));
	}
	PC->ClientMessage("GameplayMutators.Num: " + std::to_string(GameState->GameplayMutators.Num()) + " Aircrafts.Num: " + std::to_string(GameState->Aircrafts.Num()));
	PC->ClientMessage("===== End GameState Dump =====");
}

void UFortCheatManager::SpawnExitCraft(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortGameStateAthena* GameState = World->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		PC->ClientMessage("GameState is null or not an AFortGameStateAthena!");
		return;
	}

	AFortAthenaMutator_Heist* HeistMutator = nullptr;
	for (AFortAthenaMutator* Mutator : GameState->GameplayMutators) {
		if (Mutator && (HeistMutator = Mutator->Cast<AFortAthenaMutator_Heist>())) {
			break;
		}
	}

	if (!HeistMutator) {
		PC->ClientMessage("No AFortAthenaMutator_Heist in GameplayMutators! Are you on a Getaway playlist?");
		return;
	}

	UFortAthenaExitCraftInfo* CraftInfo = HeistMutator->ExitCraftInfo;
	if (!CraftInfo) {
		PC->ClientMessage("HeistMutator->ExitCraftInfo is null!");
		return;
	}

	AFortPlayerPawn* Pawn = PC->Pawn->Cast<AFortPlayerPawn>();
	if (!Pawn) {
		PC->ClientMessage("Pawn is null!");
		return;
	}

	FVector ForwardVector = Pawn->GetActorForwardVector();
	ForwardVector.Z = 0.0f;

	FVector SpawnLoc = Pawn->K2_GetActorLocation() + ForwardVector * 800.0f;

	bool bUseSpawner = Parser.GetArgBool("bUseSpawner", 0, false);

	if (bUseSpawner) {
		UClass* SpawnerClass = CraftInfo->ExitCraftSpawnerClass.Get();
		if (!SpawnerClass) {
			PC->ClientMessage("ExitCraftSpawnerClass is null on " + CraftInfo->GetName().ToString());
			return;
		}

		AFortAthenaExitCraftSpawner* Spawner = (AFortAthenaExitCraftSpawner*)World->SpawnActor(SpawnerClass, SpawnLoc);
		if (!Spawner) {
			PC->ClientMessage("Failed to spawn " + SpawnerClass->GetName().ToString() + "!");
			return;
		}

		if (!Spawner->ExitCraftInfo) {
			Spawner->ExitCraftInfo = CraftInfo;
		}

		Spawner->StartExitCraftSpawnTimer();

		PC->ClientMessage("Spawned " + Spawner->GetName().ToString() + " and started its spawn timer!");
		return;
	}

	UClass* CraftClass = CraftInfo->ExitCaftClass.Get();
	if (!CraftClass) {
		PC->ClientMessage("ExitCaftClass is null on " + CraftInfo->GetName().ToString());
		return;
	}

	float ZOffset = Parser.GetArgFloat("ZOffset", 1, 0.0f);
	SpawnLoc.Z += ZOffset;

	AFortAthenaExitCraft* Craft = (AFortAthenaExitCraft*)World->SpawnActor(CraftClass, SpawnLoc);
	if (!Craft) {
		PC->ClientMessage("Failed to spawn " + CraftClass->GetName().ToString() + "!");
		return;
	}

	if (!Craft->ExitCraftInfo) {
		Craft->ExitCraftInfo = CraftInfo;
	}

	uint8 TargetState = (uint8)Parser.GetArgInt("State", 2, 6);
	Craft->CurrentState = TargetState;
	Craft->OnNewState(TargetState);
	Craft->OnRep_CurrentState();

	Craft->ForceNetUpdate();

	PC->ClientMessage("Spawned exit craft " + Craft->GetName().ToString() + " CurrentState=" + std::to_string(Craft->CurrentState));
}

void UFortCheatManager::ActivateRifts(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	if (!ABuildingRift::StaticClass()) {
		PC->ClientMessage("BuildingRift class not found on this build!");
		return;
	}

	float Radius = Parser.GetArgFloat("Radius", 0, 0.f);
	FVector Origin = PC->Pawn ? PC->Pawn->K2_GetActorLocation() : FVector{};

	int32 Found = 0;
	int32 Activated = 0;

	for (ABuildingRift* Rift : TObjectRange<ABuildingRift>())
	{
		if (!Rift || Rift->IsDefaultObject() || Rift->GetWorld() != World)
			continue;

		Found++;

		if (Radius > 0.f && PC->Pawn) {
			if (Rift->GetDistanceTo(PC->Pawn) > Radius)
				continue;
		}

		//Rift->OnPlaced();
		//Rift->OnActivated();
		Activated++;

		PC->ClientMessage("Activated " + Rift->GetName().ToString()
			+ " (Class=" + Rift->GetClass()->GetName().ToString()
			+ " Slots=" + std::to_string(Rift->FreeSpawnSlots.Num()) + ")");
	}

	PC->ClientMessage("ActivateRifts: " + std::to_string(Activated) + " of " + std::to_string(Found) + " rifts activated.");
}

void UFortCheatManager::DumpEncounters(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortAIDirector* AIDirector = AFortAIDirector::GetCurrent(World);
	if (!AIDirector) {
		PC->ClientMessage("No AIDirector in this world!");
		return;
	}

	PC->ClientMessage("===== AI Director Dump =====");
	PC->ClientMessage("Director: " + AIDirector->GetName().ToString());

	if (AIDirector->_HasRiftSpawnPoints())
		PC->ClientMessage("RiftSpawnPoints: " + std::to_string(AIDirector->RiftSpawnPoints.Num()));
	if (AIDirector->_HasMaxActiveAlive())
		PC->ClientMessage("MaxActiveAlive: " + std::to_string(AIDirector->MaxActiveAlive) + " NumActiveAlive: " + std::to_string(AIDirector->NumActiveAlive));

	if (!AIDirector->_HasActiveEncounters()) {
		PC->ClientMessage("No ActiveEncounters property!");
		return;
	}

	PC->ClientMessage("ActiveEncounters: " + std::to_string(AIDirector->ActiveEncounters.Num()));

	for (UFortAIEncounterInfo* Encounter : AIDirector->ActiveEncounters)
	{
		if (!Encounter)
			continue;

		PC->ClientMessage("-- " + Encounter->GetName().ToString());

		if (Encounter->_HasEncounterState())
			PC->ClientMessage("   State: " + std::to_string(Encounter->EncounterState) + " Pacing: " + std::to_string(Encounter->PacingState));
		if (Encounter->_HasNumRiftsToUse())
			PC->ClientMessage("   Rifts to use: " + std::to_string(Encounter->NumRiftsToUse) + " min " + std::to_string(Encounter->MinRiftsToUse) + " used " + std::to_string(Encounter->NumRiftsUsed));
		if (Encounter->_HasCurrentSpawnPointsCap())
			PC->ClientMessage("   SpawnPoints: " + std::to_string(Encounter->CurrentSpawnPointsUsed) + " / " + std::to_string(Encounter->CurrentSpawnPointsCap));

		if (!Encounter->_HasRiftManager() || !Encounter->RiftManager) {
			PC->ClientMessage("   RiftManager: NULL");
			continue;
		}

		UFortAIEncounterRiftManager* RiftManager = Encounter->RiftManager;
		PC->ClientMessage("   RiftManager: " + RiftManager->GetName().ToString());

		if (RiftManager->_HasNumRiftsToUse())
			PC->ClientMessage("      NumRiftsToUse: " + std::to_string(RiftManager->NumRiftsToUse) + " Min: " + std::to_string(RiftManager->MinRiftsToUse));

		if (RiftManager->_HasRiftClassTemplate()) {
			UClass* RiftClass = RiftManager->RiftClassTemplate.Get();
			PC->ClientMessage("      RiftClassTemplate: " + (RiftClass ? RiftClass->GetName().ToString() : "None"));
		}

		if (!RiftManager->_HasCurrentSpawnArea()) {
			PC->ClientMessage("      No CurrentSpawnArea property!");
			continue;
		}

		const int32 RiftSize = FFortAIEncounterRift::GetSize();
		if (RiftSize <= 0) {
			PC->ClientMessage("      Failed to get the size of FFortAIEncounterRift!");
			continue;
		}

		FFortAIEncounterSpawnArea& Area = RiftManager->CurrentSpawnArea;
		PC->ClientMessage("      CurrentSpawnArea: Active=" + std::string(Area.bIsActive ? "true" : "false")
			+ " Rifts=" + std::to_string(Area.Rifts.Num())
			+ " Pending=" + std::to_string(Area.PendingRifts.Num()));

		for (int32 i = 0; i < Area.Rifts.Num(); i++)
		{
			FFortAIEncounterRift& Rift = Area.Rifts.GetWithSize(i, RiftSize);
			ABuildingRift* RiftActor = Rift.RiftActor;

			PC->ClientMessage("      [" + std::to_string(i) + "] "
				+ (RiftActor ? RiftActor->GetName().ToString() : "NULL")
				+ " QueryID=" + std::to_string(Rift.QueryID)
				+ (RiftActor && RiftActor->_HasCosmeticState() ? " CosmeticState=" + std::to_string(RiftActor->CosmeticState) : "")
				+ (RiftActor && RiftActor->_HasbRiftIsActive() ? " Active=" + std::string(RiftActor->bRiftIsActive ? "true" : "false") : ""));
		}
	}

	PC->ClientMessage("===== End AI Director Dump =====");
}

void UFortCheatManager::StartEncounter(FCommandParser& Parser)
{
	AFortPlayerController* PC = GetPlayerController();
	if (!PC)
		return;

	UWorld* World = UWorld::GetWorld();
	if (!World)
		return;

	AFortAIDirector* AIDirector = AFortAIDirector::GetCurrent(World);
	if (!AIDirector) {
		PC->ClientMessage("No AIDirector in this world!");
		return;
	}

	UClass* EncounterClass = nullptr;

	std::string EncounterClassName = Parser.GetArg("EncounterClassName", 0);
	if (!EncounterClassName.empty()) {
		UObject* EncounterObj = Utils::GetObjectFromString(EncounterClassName, UClass::StaticClass());
		EncounterClass = EncounterObj ? (UClass*)EncounterObj : nullptr;

		if (!EncounterClass) {
			PC->ClientMessage("Encounter class not found: " + EncounterClassName);
			return;
		}
	}

	if (!EncounterClass && AIDirector->_HasBaseEncounterClass())
		EncounterClass = AIDirector->BaseEncounterClass.Get();

	if (!EncounterClass && AIDirector->_HasDefaultNightEncounter())
		EncounterClass = AIDirector->DefaultNightEncounter.Get();

	if (!EncounterClass) {
		PC->ClientMessage("No encounter class to start! Pass one by name, the director has neither BaseEncounterClass nor DefaultNightEncounter set.");
		return;
	}

	PC->ClientMessage("Starting encounter from " + EncounterClass->GetName().ToString() + "...");

	UFortAIEncounterInfo* Encounter = AIDirector->StartEncounterWithoutObjective(EncounterClass);
	if (!Encounter) {
		PC->ClientMessage("StartEncounterWithoutObjective returned null.");
		return;
	}

	PC->ClientMessage("Started encounter: " + Encounter->GetName().ToString());

	if (Encounter->_HasEncounterState())
		PC->ClientMessage("State: " + std::to_string(Encounter->EncounterState) + " Pacing: " + std::to_string(Encounter->PacingState));

	if (Encounter->_HasRiftManager())
		PC->ClientMessage("RiftManager: " + std::string(Encounter->RiftManager ? Encounter->RiftManager->GetName().ToString() : "NULL"));

	PC->ClientMessage("Run DumpEncounters to see what it claimed.");
}
