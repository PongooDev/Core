#include "pch.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);
}

void AFortPlayerControllerAthena::ClientOnPawnDied(AFortPlayerControllerAthena* This, FFortPlayerDeathReport& DeathReport) {
	if (This->IsA(AFortPlayerControllerAthena::StaticClass())) {
		if (This->WorldInventory) {
			This->WorldInventory->DropAllItems();
		}
	}

	ClientOnPawnDiedOG(This, DeathReport);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ClientOnPawnDied: World is null!");
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (FortGameModeAthena && FortGameStateAthena) {
		TArray<FString> Medals;

		UFortQuestManager* QuestManager = This->GetQuestManager(ESubGame::GetAthena());

		AFortPlayerStateAthena* PlayerStateAthena = This->PlayerState->Cast<AFortPlayerStateAthena>();
		int32 MinutesAlive = PlayerStateAthena ? (PlayerStateAthena->SecondsAlive / 60) : -1;
		int32 PersonalKills = PlayerStateAthena ? PlayerStateAthena->KillScore : -1;
		int32 TeamKills = PlayerStateAthena ? PlayerStateAthena->TeamKillScore : -1;
		int32 Placement = PlayerStateAthena ? PlayerStateAthena->Place : -1;

		if (This->AthenaProfile) {
			FDedicatedServerUrlContext Context;
			This->AthenaProfile->EndBattleRoyaleGame(
				QuestManager->PendingChanges,
				FortGameModeAthena->CurrentPlaylistId,
				MinutesAlive,
				PersonalKills,
				TeamKills,
				Placement,
				Medals,
				&Context
			);
		}
	}
}

void AFortPlayerControllerAthena::OnReadyToStartMatch(AFortPlayerControllerAthena* This) {
	OnReadyToStartMatchOG(This);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortPlayerControllerAthena::OnReadyToStartMatch: World is null!");
		return;
	}

	if (This->CustomizationLoadout.Pickaxe) {
		This->WorldInventory->AddItem(This->CustomizationLoadout.Pickaxe->WeaponDefinition);
	}
}

void AFortPlayerControllerAthena::ServerReturnToMainMenu(AFortPlayerControllerAthena* This) {
	if (Version::Fortnite_Version <= 1.72) {
		return ServerReturnToMainMenuOG(This);
	}

	AFortPlayerControllerZone::ServerReturnToMainMenuOG(This);
}