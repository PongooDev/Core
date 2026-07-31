#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerGameplay.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/FortWorldItem.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"

void AFortPlayerControllerGameplay::execStartGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack) {
	UFortWorldItemDefinition* ItemProvidingGhostMode = nullptr;

	Stack.StepCompiledIn(&ItemProvidingGhostMode);
	auto Inventory = Context->WorldInventory;

	// we prob need to remove the pickaxe or smth bc its removed after the og call which is kinda ahh
	FFortItemEntry* ItemEntry = Inventory->AddItem(ItemProvidingGhostMode);
	Inventory->Update();
	Context->ServerExecuteInventoryItem(Context, ItemEntry->ItemGuid);
	((AFortPlayerControllerAthena*)Context)->ClientEquipItem(ItemEntry->ItemGuid);

	execStartGhostModeOG(Context, Stack);
}

void AFortPlayerControllerGameplay::execEndGhostMode(AFortPlayerControllerGameplay* Context, FFrame& Stack)
{
	AFortInventory* Inventory = Context->WorldInventory;
	FFortItemEntry* ItemEntry = Inventory->FindItemEntry(Context->GhostModeRepData.GhostModeItemDef, false);

	std::cout << Context->GhostModeRepData.GhostModeItemDef->GetName().ToString().c_str() << std::endl;
	//Inventory->RemoveItem(ItemEntry->ItemGuid, 1, true);
	//Inventory->Update();

	execEndGhostModeOG(Context, Stack);
}

void AFortPlayerControllerGameplay::Hook() {
	ExecHook("Function /Script/FortniteGame.FortPlayerControllerGameplay.StartGhostMode", execStartGhostMode, execStartGhostModeOG);

	ExecHook("Function /Script/FortniteGame.FortPlayerControllerGameplay.EndGhostMode", execEndGhostMode, execEndGhostModeOG);
}