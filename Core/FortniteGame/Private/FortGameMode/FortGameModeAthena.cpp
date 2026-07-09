#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortWorld/FortWorldManager.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/BuildingActor/BuildingContainer.h"
#include "FortniteGame/Public/FortInventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWorldItemDefinition.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortItemDefinition/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortQuest/FortQuestManager.h"
#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/FortTeamInfo.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistManager.h"
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"
#include "FortniteGame/Public/FortSupplyDrop/FortSupplyDropInfo.h"

bool AFortGameModeAthena::ReadyToStartMatch(AFortGameModeAthena* This) {
	if (This->bWorldIsReady
		&& This->MatchState == MatchState::WaitingToStart
		&& This->NumPlayers >= This->WarmupRequiredPlayerCount) {

		Log(std::format("ReadyToStartMatch: Match ready to start! Players={}/{}, WorldReady={}",
			This->NumPlayers,
			This->WarmupRequiredPlayerCount,
			This->bWorldIsReady));
		return true;
	}

	return false;
}

APawn* AFortGameModeAthena::SpawnDefaultPawnFor(AFortGameModeAthena* This, AController* NewPlayer, AActor* StartSpot) {
	APawn* Pawn = AFortGameModeZone::SpawnDefaultPawnFor(This, NewPlayer, StartSpot);
	if (!Pawn) {
		// sometimes it doesent work when you get kicked from the bus because the startspot is obstructed by another pawn
		Pawn = This->SpawnDefaultPawnAtTransform(NewPlayer, StartSpot->GetTransform());
	}

	Log("AFortGameModeAthena::SpawnDefaultPawnFor: Spawned default pawn. NewPlayer=" + (NewPlayer ? NewPlayer->GetName().ToString() : "None") + " Pawn=" + (Pawn ? Pawn->GetName().ToString() : "None"));
	return Pawn;
}

void AFortGameModeAthena::FinishWorldInitialization(AFortGameModeAthena* This, AFortWorldManager* WorldManager) {
	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::FinishWorldInitialization: GameState is null or not AFortGameStateAthena");
		return FinishWorldInitializationOG(This, WorldManager);
	}
	
	GameState->OnRep_CurrentPlaylistData();
	GameState->OnRep_CurrentPlaylistInfo();
	
	FinishWorldInitializationOG(This, WorldManager);
	AFortGameModeZone::FinishWorldInitialization(This, WorldManager);

	This->DefaultPawnClass = (UClass*)StaticLoadObject("/Game/Athena/PlayerPawn_Athena.PlayerPawn_Athena_C");
	//This->PlayerControllerClass = (UClass*)StaticLoadObject("/Game/Athena/Athena_PlayerController.Athena_PlayerController_C");

	This->bDisableGCOnServerDuringMatch = true;

	if (GameState->MapInfo) {
		GameState->MapInfo->SpawnLlamas();
	}
	else {
		Log("AFortGameModeAthena::FinishWorldInitialization: MapInfo is null");
	}

	if (UAthenaBattleBusItemDefinition* BBID = GetBattleBusItemDefinition()) {
		GameState->DefaultBattleBus = BBID;
		Log("Set Custom Battle Bus: " + BBID->GetName().ToString());
	}
	if (UClass* SupplyDropClass = GetSupplyDropClass()) {
		GameState->MapInfo->SupplyDropClass = SupplyDropClass;

		for (UFortSupplyDropInfo* SupplyDropInfo : GameState->MapInfo->SupplyDropInfoList) {
			SupplyDropInfo->SupplyDropClass = SupplyDropClass;
		}

		Log("Set Custom Supply Drop Class: " + SupplyDropClass->GetName().ToString());
	}

	GameState->OnRep_CurrentPlaylistData();
	GameState->OnRep_CurrentPlaylistInfo();
}

void AFortGameModeAthena::AddToAlivePlayers(AFortPlayerControllerAthena* PC) {
	void (*AddToAlivePlayersInternal)(AFortGameModeAthena* This, AFortPlayerControllerAthena* PC) = decltype(AddToAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_AddToAlivePlayers());
	AddToAlivePlayersInternal(this, PC);
}

void AFortGameModeAthena::RemoveFromAlivePlayers(AFortPlayerControllerAthena* PC, APlayerState* RemovalInstigator, APawn* FinisherPawn, UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause, bool bIsTeamSwitching) {
	void (*RemoveFromAlivePlayersInternal)(AFortGameModeAthena * This, AFortPlayerControllerAthena * PC, APlayerState * RemovalInstigator, APawn * FinisherPawn, UFortWeaponItemDefinition * FinishingWeapon, uint8 DeathCause, bool bIsTeamSwitching) = decltype(RemoveFromAlivePlayersInternal)(ImageBase + Finder::FindAFortGameModeAthena_RemoveFromAlivePlayers());
	RemoveFromAlivePlayersInternal(this, PC, RemovalInstigator, FinisherPawn, FinishingWeapon, DeathCause, bIsTeamSwitching);
}

int32 AFortGameModeAthena::StartAircraftPhase(AFortGameModeAthena* This, bool bGoStraightToSafeZone) {
	for (AFortPlayerControllerAthena* PC : This->AlivePlayers) {
		if (PC->WorldInventory) {
			PC->WorldInventory->DropAllItems(false);
		}

		if (PC->MyFortPawn || PC->Pawn->Cast<AFortPlayerPawn>()) {
			AFortPlayerPawn* MyFortPawn = PC->MyFortPawn ? PC->MyFortPawn : PC->Pawn->Cast<AFortPlayerPawn>();

			MyFortPawn->SetHealth(MyFortPawn->GetMaxHealth());
			MyFortPawn->SetShield(0.0f);
		}
	}

	return StartAircraftPhaseOG(This, bGoStraightToSafeZone);
}

uint8 AFortGameModeAthena::PickTeam(AFortGameModeAthena* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor) {
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bDevSameTeam) {
		return 2;
	}
	if (Config.bUseGameSessions) {
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PickTeam: GameState is null or not AFortGameStateAthena");
		return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
	}

	for (AFortTeamInfo* TeamInfo : GameState->Teams) {
		if (!TeamInfo) {
			continue;
		}

		int32 TeamMemberCount = TeamInfo->TeamMembers.Num();
		if (TeamMemberCount < GameState->TeamSize) {
			Log("AFortGameModeAthena::PickTeam: Assigning player to team " + std::to_string(TeamInfo->Team) + " with " + std::to_string(TeamMemberCount) + " members.");
			return TeamInfo->Team;
		}
	}

	return PickTeamOG(This, PreferredTeam, ControllerToPickFor);
}

void AFortGameModeAthena::InitGameState(AFortGameModeAthena* This) {
	InitGameStateOG(This);
	
	FCoreConfig& Config = ConfigurationManager::GetConfig();

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::InitGameState: GameState is null or not AFortGameStateAthena");
		return;
	}

	if (UFortPlaylistManager::StaticClass()) {
		UFortPlaylistManager* PlaylistManager = UFortPlaylistManager::Get();
		UFortPlaylistAthena* Playlist = nullptr;
		if (PlaylistManager) {
			// We need to check if Config.Playlist is a number or a string and thats how we will find the playlist
			if (Config.Playlist.find_first_not_of("0123456789") == std::string::npos) {
				int32 PlaylistId = std::stoi(Config.Playlist);
				Playlist = PlaylistManager->GetPlaylist(PlaylistId);
			}
			else {
				FName PlaylistName = UKismetStringLibrary::Conv_StringToName(Config.Playlist);
				Playlist = PlaylistManager->GetPlaylist(PlaylistName);
			}
		}
		else {
			Log("AFortGameModeAthena::InitGameState: Failed to get PlaylistManager");
		}

		if (Playlist) {
			if (GameState->_HasCurrentPlaylistData()) {
				GameState->CurrentPlaylistData = Playlist;
			}
			if (GameState->_HasCurrentPlaylistInfo()) {
				GameState->CurrentPlaylistInfo.BasePlaylist = Playlist;
				GameState->CurrentPlaylistInfo.OverridePlaylist = Playlist;
				GameState->CurrentPlaylistInfo.PlaylistReplicationKey++;
				GameState->CurrentPlaylistInfo.MarkArrayDirty();
			}

			This->CurrentPlaylistName = Playlist->GetPlaylistName();
			This->CurrentPlaylistId = Playlist->GetPlaylistId();
			GameState->SetCurrentPlaylistId(This->CurrentPlaylistId);

			GameState->TeamCount = Playlist->MaxTeamCount;
			GameState->TeamSize = Playlist->MaxTeamSize;
			GameState->bIsLargeTeamGame = Playlist->bIsLargeTeamGame;

			This->GameSession->MaxPlayers = Playlist->MaxPlayers;
			This->GameSession->MaxPartySize = Playlist->MaxTeamSize;

			This->MaxPlayerCount = Playlist->MaxPlayers;

			GameState->CachedSafeZoneStartUp = Playlist->SafeZoneStartUp;

			Log("AFortGameModeAthena::InitGameState: Applied playlist " + Playlist->GetFName().ToString().ToString());

			// Start Playlist Dump
			Log("====== Playlist Dump ======");
			Log("Playlist: " + Playlist->GetFName().ToString().ToString());
			Log("PlaylistName: " + Playlist->GetPlaylistName().ToString().ToString());
			Log("PlaylistId: " + std::to_string(Playlist->GetPlaylistId()));
			Log("MaxTeamCount: " + std::to_string(Playlist->MaxTeamCount));
			Log("MaxTeamSize: " + std::to_string(Playlist->MaxTeamSize));
			Log("MaxSquadSize: " + std::to_string(Playlist->MaxSquadSize));
			Log("MaxPlayers: " + std::to_string(Playlist->MaxPlayers));
			Log("SafeZoneStartUp: " + std::to_string(Playlist->SafeZoneStartUp));
			Log("bIsLargeTeamGame: " + std::string(Playlist->bIsLargeTeamGame ? "true" : "false"));
			Log("====== End Playlist Dump ======");
		}
		else {
			Log("AFortGameModeAthena::InitGameState: Failed to get Playlist");
		}
	}
	else {
		GameState->SetCurrentPlaylistId(This->CurrentPlaylistId);

		This->GameSession->MaxPlayers = 100;
		This->GameSession->MaxPartySize = GameState->TeamSize;

		This->MaxPlayerCount = 100;
	}
}

UAthenaBattleBusItemDefinition* AFortGameModeAthena::GetBattleBusItemDefinition() {
	UAthenaBattleBusItemDefinition* BBID = nullptr;
	if (Version::Fortnite_Version == 1.11
		|| (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 2.42)) {
		BBID = (UAthenaBattleBusItemDefinition*)StaticLoadObject("/Game/Athena/Items/Cosmetics/BattleBuses/BBID_WinterBus.BBID_WinterBus");
	}

	return BBID;
}

UClass* AFortGameModeAthena::GetSupplyDropClass() {
	UClass* SupplyDropClass = nullptr;
	if (Version::Fortnite_Version == 1.11
		|| (Version::Fortnite_Version >= 2.1 && Version::Fortnite_Version <= 2.42)) {
		SupplyDropClass = (UClass*)StaticLoadObject("/Game/Athena/SupplyDrops/B_AthenaSupplyDrop_Gift.B_AthenaSupplyDrop_Gift_C");
	}

	return SupplyDropClass;
}

void AFortGameModeAthena::PlacePlayerOnTeam(AFortGameModeAthena* This, AFortPlayerController* FortPC) {
	PlacePlayerOnTeamOG(This, FortPC);
	if (!FortPC) {
		return;
	}

	AFortGameStateAthena* GameState = This->GameState->Cast<AFortGameStateAthena>();
	if (!GameState) {
		Log("AFortGameModeAthena::PlacePlayerOnTeam: GameState is null or not AFortGameStateAthena");
		return;
	}

	AFortPlayerStateAthena* FortPS = FortPC->PlayerState->Cast<AFortPlayerStateAthena>();
	if (!FortPS) {
		return;
	}

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (GameState->TeamSize > 1 && !Config.bUseGameSessions) {
		FortPS->SquadId = FortPS->TeamIndex - 3;
		FortPS->OnRep_SquadId();

		Log("AFortGameModeAthena::PlacePlayerOnTeam: Set SquadId for PlayerState: " + FortPS->GetName().ToString() + " to " + std::to_string(FortPS->SquadId));
	}
}