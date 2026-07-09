#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/FortTeamInfo.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/Athena/AthenaRewardResult.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"
#include "FortniteGame/Public/Athena/AthenaMatchTeamStats.h"
#include "FortniteGame/Public/Athena/AthenaPlayerMatchReport.h"

void AFortPlayerControllerZone::ServerAcknowledgePossession(AFortPlayerControllerZone* This, AFortPlayerPawnAthena* P) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerZone::ServerAcknowledgePossession: World is null!");
		return AFortPlayerController::ServerAcknowledgePossessionOG(This, P);
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();

	auto PlayerState = (AFortPlayerStateZone*)This->PlayerState;
	if (PlayerState) {
		if (PlayerState->AbilitySystemComponent) {
			UFortAbilitySet* FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");

			if (FortGameModeAthena) {
				UFortAbilitySet* AthenaPlayerAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
				if (AthenaPlayerAbilitySet) {
					FortAbilitySet = AthenaPlayerAbilitySet;
				}
			}

			PlayerState->AbilitySystemComponent->GiveAbilitySet(FortAbilitySet);
		}
	}

	AFortPlayerController::ServerAcknowledgePossessionOG(This, P);
}

void AFortPlayerControllerZone::OnReadyToStartMatch(AFortPlayerControllerZone* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerZone::OnReadyToStartMatch: World is null!");
		return;
	}

	AFortGameModeZone* FortGameModeZone = World->AuthorityGameMode->Cast<AFortGameModeZone>();
	if (!FortGameModeZone) {
		Log("AFortPlayerControllerZone::OnReadyToStartMatch: GameMode is null or not a FortGameModeZone!");
		return;
	}

	AFortGameStateZone* FortGameStateZone = World->GameState->Cast<AFortGameStateZone>();
	if (!FortGameStateZone) {
		Log("AFortPlayerControllerZone::OnReadyToStartMatch: GameState is null or not a FortGameStateZone!");
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (Version::Fortnite_Version < 1.8) {
		if (This->MainMcpProfile) {
			FDedicatedServerUrlContext Context;
			This->MainMcpProfile->ServerQuestLogin(FortGameStateZone->GameSessionID, &Context);
		}
	}
	else {
		if (This->AthenaProfile) {
			FDedicatedServerUrlContext Context;
			This->AthenaProfile->ServerQuestLogin(FortGameStateZone->GameSessionID, &Context);
		}
	}

	AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>();
	if (FortPCAthena) {
		if (FortPCAthena->CustomizationLoadout.Pickaxe) {
			FortPCAthena->WorldInventory->AddItem(FortPCAthena->CustomizationLoadout.Pickaxe->WeaponDefinition);
		}
	}

	if (FortGameModeZone->_HasStartingItems() && FortGameModeZone->StartingItems.Num() > 0)
	{
		Log("OnReadyToStartMatch: Processing StartingItems for new player. Count: " + std::to_string(FortGameModeZone->StartingItems.Num()));
		for (int i = 0; i < FortGameModeZone->StartingItems.Num(); i++)
		{
			auto& StartingItem = FortGameModeZone->StartingItems.GetWithSize(i, FItemAndCount::GetSize());

			Log(std::format("StartingItem {}: Item={}, Count={}", i, StartingItem.Item ? StartingItem.Item->GetFName().ToString().ToString() : "None", StartingItem.Count));
			This->WorldInventory->AddItem(StartingItem.Item, StartingItem.Count);
		}
	}
	else {
		Log(" Warning: No StartingItems found!");

		static UFortItemDefinition* DefaultPickaxe = DefaultPickaxe = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/Melee/Harvest/WID_Harvest_Pickaxe_C_T01.WID_Harvest_Pickaxe_C_T01");

		static UFortItemDefinition* WallBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Wall.BuildingItemData_Wall");
		static UFortItemDefinition* FloorBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Floor.BuildingItemData_Floor");
		static UFortItemDefinition* StairBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_Stair_W.BuildingItemData_Stair_W");
		static UFortItemDefinition* ConeBuild = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/BuildingItemData_RoofS.BuildingItemData_RoofS");
		static UFortItemDefinition* EditTool = (UFortItemDefinition*)StaticLoadObject("/Game/Items/Weapons/BuildingTools/EditTool.EditTool");

		if (DefaultPickaxe) {
			This->WorldInventory->AddItem(DefaultPickaxe);
		}
		if (WallBuild) {
			This->WorldInventory->AddItem(WallBuild);
		}
		if (FloorBuild) {
			This->WorldInventory->AddItem(FloorBuild);
		}
		if (StairBuild) {
			This->WorldInventory->AddItem(StairBuild);
		}
		if (ConeBuild) {
			This->WorldInventory->AddItem(ConeBuild);
		}
		if (EditTool) {
			This->WorldInventory->AddItem(EditTool);
		}
	}
}

void AFortPlayerControllerZone::ServerReturnToMainMenu(AFortPlayerControllerZone* This) {
	ServerReturnToMainMenuOG(This);
}

void AFortPlayerControllerZone::ClientOnPawnDied_Implementation(FFortPlayerDeathReport& DeathReport) {
	
}

void AFortPlayerControllerZone::ClientOnPawnDied(AFortPlayerControllerZone* This, FFortPlayerDeathReport& DeathReport) {
	if (AFortPlayerControllerAthena* FortPCAthena = This->Cast<AFortPlayerControllerAthena>()) {
		FortPCAthena->ClientOnPawnDied_Implementation(DeathReport);
	}

	This->ClientOnPawnDied_Implementation(DeathReport);
	ClientOnPawnDiedOG(This, DeathReport);
}

void AFortPlayerControllerZone::ClientOnPawnRevived(AController* EventInstigator) {
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientOnPawnRevived");

	if (!Func) {
		return;
	}

	Call(Func, EventInstigator);
}

void AFortPlayerControllerZone::Hook() {
	HookEveryVTable(AFortPlayerControllerZone::StaticClass(), AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), ServerAcknowledgePossession, nullptr);
	
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_OnReadyToStartMatch()), OnReadyToStartMatch, (LPVOID*)&OnReadyToStartMatchOG);

	HookVTable(
		AFortPlayerControllerZone::GetDefaultObj(),
		AFortPlayerControllerZone::StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerController.ServerReturnToMainMenu"),
		ServerReturnToMainMenu,
		(LPVOID*)&ServerReturnToMainMenuOG
	);

	MH_CreateHook((LPVOID)(ImageBase + Finder::FindAFortPlayerControllerZone_ClientOnPawnDied()), ClientOnPawnDied, (LPVOID*)&ClientOnPawnDiedOG);

	Log("Hooked AFortPlayerControllerZone");
}