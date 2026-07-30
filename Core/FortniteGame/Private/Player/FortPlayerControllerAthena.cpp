#include "pch.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Items/Definitions/AthenaEmojiItemDefinition.h"

#include "FortniteGame/Public/Inventory/FortQuickBarsAthena.h"
#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/Heroes/FortHeroType.h"
#include "FortniteGame/Public/Heroes/FortHero.h"
#include "FortniteGame/Public/Customization/CustomCharacterPart.h"
#include "FortniteGame/Public/Heroes/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"
#include "FortniteGame/Public/Mcp/McpProfileSys.h"
#include "FortniteGame/Public/FortGameStateAthena.h"
#include "FortniteGame/Public/Playlists/FortPlaylistAthena.h"
#include "FortniteGame/Public/Quests/FortQuestManager.h"
#include "FortniteGame/Public/Quests/FortQuestObjectiveCompletion.h"
#include "FortniteGame/Public/Player/FortPlayerDeathReport.h"
#include "FortniteGame/Public/FortTeamInfo.h"
#include "FortniteGame/Public/Weapons/FortWeapon.h"
#include "FortniteGame/Public/Athena/AthenaRewardResult.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"
#include "FortniteGame/Public/Athena/AthenaMatchTeamStats.h"
#include "FortniteGame/Public/Athena/AthenaPlayerMatchReport.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/Player/AthenaQuickChatBank.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponentAthena.h"
#include "FortniteGame/Public/Player/DeathInfo.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);

	if (This->WorldInventory) {
		This->WorldInventory->DropAllItems(false);
	}
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);

	if (This->MyFortPawn) {
		This->MyFortPawn->ForceNetUpdate();
	}
}

bool AFortPlayerControllerAthena::HasAliveTeamMember() {
	AFortPlayerStateAthena* PlayerStateAthena = PlayerState ? PlayerState->Cast<AFortPlayerStateAthena>() : nullptr;
	if (!PlayerStateAthena || !PlayerStateAthena->PlayerTeam) {
		return false;
	}

	for (AController* TeamMember : PlayerStateAthena->PlayerTeam->TeamMembers) {
		AFortPlayerControllerAthena* TeamMemberController = TeamMember ? TeamMember->Cast<AFortPlayerControllerAthena>() : nullptr;
		if (!TeamMemberController || TeamMemberController == this || !TeamMemberController->bMarkedAlive) {
			continue;
		}

		AFortPlayerPawnAthena* TeamMemberPlayerPawn = TeamMemberController->MyFortPawn
			? TeamMemberController->MyFortPawn->Cast<AFortPlayerPawnAthena>() : nullptr;
		if (!TeamMemberPlayerPawn || TeamMemberPlayerPawn->bIsDBNO) {
			continue;
		}

		return true;
	}

	return false;
}

void AFortPlayerControllerAthena::ClientOnPawnDied_Implementation(FFortPlayerDeathReport& DeathReport) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ClientOnPawnDied: World is null!");
		return;
	}

	AFortPlayerControllerZone::ClientOnPawnDied_Implementation(DeathReport);

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (!FortGameModeAthena || !FortGameStateAthena) {
		Log("ClientOnPawnDied: GameMode or GameState is null or not a FortGameModeAthena/FortGameStateAthena!");
		return;
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerStateAthena) {
		Log("ClientOnPawnDied: PlayerState is null or not a FortPlayerStateAthena!");
		return;
	}

	UFortAbilitySystemComponentAthena* AbilitySystemComponentAthena = 
		PlayerStateAthena ? PlayerStateAthena->AbilitySystemComponent->Cast<UFortAbilitySystemComponentAthena>() : nullptr;

	AFortPlayerPawnAthena* PlayerPawnAthena = MyFortPawn->Cast<AFortPlayerPawnAthena>();
	if (!PlayerPawnAthena) {
		Log("ClientOnPawnDied: MyFortPawn is null or not a FortPlayerPawnAthena!");
		return;
	}

	AFortPlayerState* KillerPlayerState = DeathReport.KillerPlayerState;
	AFortPawn* KillerPawn = DeathReport.KillerPawn;

	AFortPlayerStateAthena* KillerPlayerStateAthena = DeathReport.KillerPlayerState->Cast<AFortPlayerStateAthena>();
	AFortPlayerPawnAthena* KillerPawnAthena = DeathReport.KillerPawn->Cast<AFortPlayerPawnAthena>();
	UFortWeaponItemDefinition* FinishingWeapon = DeathReport.DamageCauser->IsA(AFortWeapon::StaticClass())
		? DeathReport.DamageCauser->Cast<AFortWeapon>()->WeaponData : nullptr;

	bool bHasAliveTeamMember = false;
	bool bIsDBNO = PlayerPawnAthena->bIsDBNO || PlayerPawnAthena->bWasDBNOOnDeath;

	if (PlayerStateAthena && PlayerStateAthena->PlayerTeam) {
		for (AController* TeamMember : PlayerStateAthena->PlayerTeam->TeamMembers) {
			AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
			if (!TeamMemberController || TeamMemberController == this || !TeamMemberController->bMarkedAlive) {
				continue;
			}

			AFortPlayerPawnAthena* TeamMemberPlayerPawn = TeamMemberController->MyFortPawn->Cast<AFortPlayerPawnAthena>();
			if (!TeamMemberPlayerPawn || TeamMemberPlayerPawn->bIsDBNO) {
				continue;
			}

			bHasAliveTeamMember = true;
			break;
		}
	}

	EDeathCause DeathCause = AFortPlayerStateAthena::ToDeathCause(DeathReport.Tags, bIsDBNO);
	FVector PawnDeathLocation = Pawn ? Pawn->K2_GetActorLocation() : FVector{};
	float Distance = DeathCause != EDeathCause::GetFallDamage()
		? ((KillerPawn && Pawn) ? KillerPawn->GetDistanceTo(Pawn) : 0)
		: PlayerPawnAthena->LastFallDistance;

	PlayerStateAthena->DeathInfo.FinisherOrDowner = KillerPlayerStateAthena ? KillerPlayerStateAthena : PlayerStateAthena;
	PlayerStateAthena->DeathInfo.bDBNO = bIsDBNO;
	PlayerStateAthena->DeathInfo.DeathCause = DeathCause;
	PlayerStateAthena->DeathInfo.Distance = Distance;
	PlayerStateAthena->DeathInfo.DeathLocation = PawnDeathLocation;
	PlayerStateAthena->DeathInfo.bInitialized = true;
	PlayerStateAthena->OnRep_DeathInfo();

	if (WorldInventory) {
		WorldInventory->DropAllItems();
	}

	if (Version::Fortnite_Version >= 1.8 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		if (KillerPlayerStateAthena && KillerPlayerStateAthena != PlayerStateAthena) {
			KillerPlayerStateAthena->SetKillScore(KillerPlayerStateAthena->KillScore + 1);
			KillerPlayerStateAthena->ClientReportKill(PlayerStateAthena);

			if (KillerPlayerStateAthena->PlayerTeam) {
				for (AController* TeamMember : KillerPlayerStateAthena->PlayerTeam->TeamMembers) {
					AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
					if (TeamMemberController) {
						AFortPlayerStateAthena* TeamMemberPlayerState = TeamMemberController->PlayerState->Cast<AFortPlayerStateAthena>();
						if (TeamMemberPlayerState) {
							TeamMemberPlayerState->TeamKillScore++;
							TeamMemberPlayerState->ClientReportTeamKill(TeamMemberPlayerState->TeamKillScore);
							TeamMemberPlayerState->OnRep_TeamKillScore();
						}
					}
				}
			}
		}
	}

	bool IsRespawningAllowed = FortGameStateAthena->IsRespawningAllowed(PlayerStateAthena);

	if (Version::Fortnite_Version >= 1.91 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		if (!IsRespawningAllowed) {
			if (!bHasAliveTeamMember) {
				if (PlayerStateAthena && PlayerStateAthena->PlayerTeam) {
					for (AController* TeamMember : PlayerStateAthena->PlayerTeam->TeamMembers) {
						AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
						if (!TeamMemberController) {
							continue;
						}

						AFortPlayerStateAthena* TeamMemberPlayerState = TeamMemberController->PlayerState->Cast<AFortPlayerStateAthena>();
						if (!TeamMemberPlayerState) {
							continue;
						}

						TeamMemberPlayerState->Place = FortGameStateAthena->TeamsLeft; // we wanna do this before removing the player from alive players, so that the place is correct
						TeamMemberPlayerState->OnRep_Place();

						AFortGameModeAthena::SendEndOfMatchTo(TeamMemberController);
					}
				}
			}

			FortGameModeAthena->RemoveFromAlivePlayers(
				this,
				KillerPlayerStateAthena == PlayerStateAthena ? nullptr : KillerPlayerStateAthena,
				KillerPawnAthena,
				FinishingWeapon,
				PlayerStateAthena->DeathInfo.DeathCause,
				false
			);
		}
	}
	if ((Version::Fortnite_Version <= 6.0 && Version::Fortnite_Version >= 1.91) || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		if (FortGameModeAthena->bAllowSpectateAfterDeath) {
			APawn* PawnToSpectate = DeathReport.KillerPawn;
			if (!PawnToSpectate && PlayerStateAthena->PlayerTeam) {
				// check if there is any team member alive to spectate
				for (AController* TeamMember : PlayerStateAthena->PlayerTeam->TeamMembers) {
					AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
					if (!TeamMemberController || TeamMemberController == this || !TeamMemberController->bMarkedAlive) {
						continue;
					}
					AFortPlayerPawnAthena* TeamMemberPlayerPawn = TeamMemberController->MyFortPawn->Cast<AFortPlayerPawnAthena>();
					if (!TeamMemberPlayerPawn) {
						continue;
					}
					PawnToSpectate = TeamMemberPlayerPawn;
					break;
				}
			}
			if (!PawnToSpectate) {
				// if no team memeber alive, then find the next alive player
				for (AFortPlayerControllerAthena* PC : FortGameModeAthena->AlivePlayers) {
					if (PC && PC != this && PC->bMarkedAlive && PC->Pawn) {
						PawnToSpectate = PC->Pawn;
						break;
					}
				}
			}

			if (PawnToSpectate) {
				PlayerToSpectateOnDeath = PawnToSpectate;

				FTimerHandle TimerHandle = UKismetSystemLibrary::K2_SetTimer(this, "SpectateOnDeath", 5.f, false);
				if (!TimerHandle.IsValid()) {
					Log("AFortPlayerControllerAthena::ClientOnPawnDied: Failed to set timer for SpectateOnDeath!");
				}
			}
			else {
				Log("AFortPlayerControllerAthena::ClientOnPawnDied: Unable to find a pawn to spectate after death!");
			}
		}
	}
}

void AFortPlayerControllerAthena::OnReadyToStartMatch(AFortPlayerControllerAthena* This) {
	OnReadyToStartMatchOG(This);
}

void AFortPlayerControllerAthena::ServerReturnToMainMenu(AFortPlayerControllerAthena* This) {
	if (Version::Fortnite_Version <= 1.72 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return ServerReturnToMainMenuOG(This);
	}

	AFortPlayerControllerZone::ServerReturnToMainMenuOG(This);
}

void AFortPlayerControllerAthena::ClientNotifyTeamWon(APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientNotifyTeamWon");

	return Call(Func, FinisherPawn, FinishingWeapon, DeathCause);
}

void AFortPlayerControllerAthena::ClientNotifyWon(APawn* FinisherPawn, const UFortWeaponItemDefinition* FinishingWeapon, uint8 DeathCause)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientNotifyWon");

	return Call(Func, FinisherPawn, FinishingWeapon, DeathCause);
}

void AFortPlayerControllerAthena::ClientEquipItem(FGuid ItemGuid)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientEquipItem");

	return Call(Func, ItemGuid);
}

void AFortPlayerControllerAthena::ClientSendEndBattleRoyaleMatchForPlayer(bool bSuccess, const FAthenaRewardResult& Result)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientSendEndBattleRoyaleMatchForPlayer");

	if (!Func) {
		return;
	}

	return Call(Func, bSuccess, Result);
}

void AFortPlayerControllerAthena::ClientSendMatchStatsForPlayer(const FAthenaMatchStats& Stats)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientSendMatchStatsForPlayer");

	if (!Func) {
		return;
	}

	return Call(Func, Stats);
}

void AFortPlayerControllerAthena::ClientSendTeamStatsForPlayer(const FAthenaMatchTeamStats& TeamStats)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientSendTeamStatsForPlayer");

	if (!Func) {
		return;
	}

	return Call(Func, TeamStats);
}

FAthenaRewardResult& AFortPlayerControllerAthena::ConstructAthenaRewardResult() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ConstructAthenaRewardResult: World is null!");
		return *FAthenaRewardResult::Allocate();
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (!FortGameModeAthena || !FortGameStateAthena) {
		Log("ConstructAthenaRewardResult: GameMode or GameState is null or not a FortGameModeAthena/FortGameStateAthena!");
		return *FAthenaRewardResult::Allocate();
	}
	
	FAthenaRewardResult* AthenaRewardResult = FAthenaRewardResult::Allocate();

	return *AthenaRewardResult;
}

FAthenaMatchStats& AFortPlayerControllerAthena::ConstructAthenaMatchStats() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ConstructAthenaMatchStats: World is null!");
		return *FAthenaMatchStats::Allocate();
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (!FortGameModeAthena || !FortGameStateAthena) {
		Log("ConstructAthenaMatchStats: GameMode or GameState is null or not a FortGameModeAthena/FortGameStateAthena!");
		return *FAthenaMatchStats::Allocate();
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerStateAthena) {
		Log("ConstructAthenaMatchStats: PlayerState is null or not a FortPlayerStateAthena!");
		return *FAthenaMatchStats::Allocate();
	}
	
	int32 Place = PlayerStateAthena->Place;
	if (Place <= 0) {
		Place = 1;
	}

	int32 TotalPlayers = FortGameStateAthena->TotalPlayers;
	if (TotalPlayers < Place) {
		TotalPlayers = Place;
	}

	int32 SecondsAlive = PlayerStateAthena->SecondsAlive;
	if (SecondsAlive <= 0) {
		SecondsAlive = FortGameStateAthena->ElapsedTime;
	}
	if (SecondsAlive < 0) {
		SecondsAlive = 0;
	}

	FAthenaMatchStats* AthenaMatchStats = FAthenaMatchStats::Allocate();
	AthenaMatchStats->Place = Place;
	AthenaMatchStats->TotalPlayers = TotalPlayers;
	AthenaMatchStats->SecondsAlive = SecondsAlive;
	AthenaMatchStats->Kills = PlayerStateAthena->KillScore;
	AthenaMatchStats->Downs = PlayerStateAthena->DownScore;

	return *AthenaMatchStats;
}

FAthenaMatchTeamStats& AFortPlayerControllerAthena::ConstructAthenaMatchTeamStats() {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ConstructAthenaMatchTeamStats: World is null!");
		return *FAthenaMatchTeamStats::Allocate();
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();
	if (!FortGameModeAthena || !FortGameStateAthena) {
		Log("ConstructAthenaMatchTeamStats: GameMode or GameState is null or not a FortGameModeAthena/FortGameStateAthena!");
		return *FAthenaMatchTeamStats::Allocate();
	}

	AFortPlayerStateAthena* PlayerStateAthena = PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerStateAthena) {
		Log("ConstructAthenaMatchTeamStats: PlayerState is null or not a FortPlayerStateAthena!");
		return *FAthenaMatchTeamStats::Allocate();
	}

	int32 Place = PlayerStateAthena->Place;
	if (Place <= 0) {
		Place = 1;
	}

	int32 TotalPlayers = FortGameStateAthena->TotalPlayers;
	if (TotalPlayers < Place) {
		TotalPlayers = Place;
	}

	FAthenaMatchTeamStats* AthenaMatchTeamStats = FAthenaMatchTeamStats::Allocate();
	AthenaMatchTeamStats->Place = Place;
	AthenaMatchTeamStats->TotalPlayers = TotalPlayers;

	return *AthenaMatchTeamStats;
}

void AFortPlayerControllerAthena::ServerPlaySquadQuickChatMessage(AFortPlayerControllerAthena* This, FAthenaQuickChatActiveEntry& ChatEntry, FUniqueNetIdRepl& SenderID) {
	ServerPlaySquadQuickChatMessageOG(This, ChatEntry, SenderID);
	
	AFortPlayerStateAthena* PlayerStateAthena = This->PlayerState->Cast<AFortPlayerStateAthena>();
	if (!PlayerStateAthena) {
		Log("ServerPlaySquadQuickChatMessage: PlayerState is null or not a FortPlayerStateAthena!");
		return;
	}
	
	UAthenaQuickChatBank* QuickChatBank = ChatEntry.GetBank();
	if (!QuickChatBank) {
		Log("ServerPlaySquadQuickChatMessage: QuickChatBank is null!");
		return;
	}

	if (!QuickChatBank->_HasChatOptions()) {
		Log("ServerPlaySquadQuickChatMessage: AthenaQuickChatBank has no ChatOptions property!");
		return;
	}

	const int32 LeafEntrySize = FAthenaQuickChatLeafEntry::GetSize();
	if (LeafEntrySize <= 0) {
		Log("ServerPlaySquadQuickChatMessage: Failed to get the size of AthenaQuickChatLeafEntry!");
		return;
	}

	if (!QuickChatBank->ChatOptions.IsValidIndex(ChatEntry.Index)) {
		Log("ServerPlaySquadQuickChatMessage: ChatEntry index is out of bounds!");
		return;
	}

	FAthenaQuickChatLeafEntry& LeafEntry = QuickChatBank->ChatOptions.GetWithSize(ChatEntry.Index, LeafEntrySize);

	ServerPlayEmoteItem(This, LeafEntry.EmojiItemDefinition, 0.f);

	PlayerStateAthena->TeamMemberState = LeafEntry.TeamCommType;
	PlayerStateAthena->ReplicatedTeamMemberState = LeafEntry.TeamCommType;
	PlayerStateAthena->OnRep_ReplicatedTeamMemberState();
}

void AFortPlayerControllerAthena::TogglePersonalVehicle(bool bOn) {
	Log("AFortPlayerControllerAthena::TogglePersonalVehicle called with bOn = " + std::to_string(bOn));
}
