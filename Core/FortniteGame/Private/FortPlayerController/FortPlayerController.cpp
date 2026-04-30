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
		if (Version::Fortnite_Version <= 1.72) {
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
			if (!QuickBars)
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
	ServerCheatOG(This, Msg);

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

	FCommandParser Parser(Command);

	if (Parser.IsCommand("Help"))
	{
		This->ClientMessage("=== Available Commands ===");
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