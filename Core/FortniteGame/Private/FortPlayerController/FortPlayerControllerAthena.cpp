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
#include "FortniteGame/Public/FortPlayer/FortPlayerDeathReport.h"
#include "FortniteGame/Public/Info/FortTeamInfo.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/Athena/AthenaRewardResult.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"
#include "FortniteGame/Public/Athena/AthenaMatchTeamStats.h"
#include "FortniteGame/Public/Athena/AthenaPlayerMatchReport.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/QuickChat/AthenaQuickChatBank.h"

void AFortPlayerControllerAthena::EnterAircraft(AFortPlayerControllerAthena* This, AFortAircraft* InAircraft) {
	EnterAircraftOG(This, InAircraft);
}

void AFortPlayerControllerAthena::ServerAttemptAircraftJump(AFortPlayerControllerAthena* This, FRotator& ClientRotation) {
	ServerAttemptAircraftJumpOG(This, ClientRotation);

	if (This->MyFortPawn) {
		This->MyFortPawn->ForceNetUpdate();
	}
}

void AFortPlayerControllerAthena::ClientOnPawnDied_Implementation(FFortPlayerDeathReport& DeathReport) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ClientOnPawnDied: World is null!");
		return;
	}

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

	AFortPlayerStateAthena* KillerPlayerStateAthena = DeathReport.KillerPlayerState->Cast<AFortPlayerStateAthena>();
	AFortPlayerPawnAthena* KillerPlayerPawnAthena = DeathReport.KillerPawn->Cast<AFortPlayerPawnAthena>();
	AFortPlayerControllerAthena* KillerPCAthena = KillerPlayerPawnAthena ? KillerPlayerPawnAthena->Controller->Cast<AFortPlayerControllerAthena>() : nullptr;
	UFortWeaponItemDefinition* FinishingWeapon = DeathReport.DamageCauser->IsA(AFortWeapon::StaticClass())
		? DeathReport.DamageCauser->Cast<AFortWeapon>()->WeaponData : nullptr;

	if (WorldInventory) {
		WorldInventory->DropAllItems();
	}

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

	if (Version::Fortnite_Version >= 1.8 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		if (KillerPlayerStateAthena && KillerPlayerStateAthena != PlayerStateAthena) {
			KillerPlayerStateAthena->KillScore++;

			if (Version::Fortnite_Version >= 1.8 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
				KillerPlayerStateAthena->ClientReportKill(PlayerStateAthena);
			}
			KillerPlayerStateAthena->OnRep_Kills();

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

			static auto AthenaKills = UKismetStringLibrary::Conv_StringToName(L"AthenaKills");
			Utils::SetClientObservedStat((AFortPlayerPawn*)MyFortPawn, AthenaKills, KillerPlayerStateAthena->KillScore);
		}
	}

	PlayerStateAthena->DeathInfo.bDBNO = bIsDBNO;
	PlayerStateAthena->DeathInfo.FinisherOrDowner = KillerPlayerStateAthena ? KillerPlayerStateAthena : PlayerStateAthena;
	PlayerStateAthena->DeathInfo.DeathCause = AFortPlayerStateAthena::ToDeathCause(DeathReport.Tags, bIsDBNO);
	/*Log("==================== DeathInfo Dump Start ====================");
	Log("==================== bDBNO=" + std::to_string(PlayerStateAthena->DeathInfo.bDBNO));
	Log("==================== FinisherOrDowner=" + (PlayerStateAthena->DeathInfo.FinisherOrDowner ? PlayerStateAthena->DeathInfo.FinisherOrDowner->GetName().ToString() : "None"));
	Log("==================== DeathCause=" + std::to_string(PlayerStateAthena->DeathInfo.DeathCause));
	Log("==================== DeathInfo Dump End ====================");*/
	PlayerStateAthena->OnRep_DeathInfo();

	if (Version::Fortnite_Version >= 1.91 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
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

					if (FAthenaRewardResult::StaticStruct()) {
						TeamMemberController->ClientSendEndBattleRoyaleMatchForPlayer(true, TeamMemberController->ConstructAthenaRewardResult());
					}

					if (FAthenaMatchStats::StaticStruct()) {
						TeamMemberController->ClientSendMatchStatsForPlayer(TeamMemberController->ConstructAthenaMatchStats());
					}
					
					if (FAthenaMatchTeamStats::StaticStruct()) {
						TeamMemberController->ClientSendTeamStatsForPlayer(TeamMemberController->ConstructAthenaMatchTeamStats());
					}
				}
			}
		}

		FortGameModeAthena->RemoveFromAlivePlayers(
			this,
			KillerPlayerStateAthena == PlayerStateAthena ? nullptr : KillerPlayerStateAthena,
			KillerPlayerPawnAthena,
			FinishingWeapon,
			PlayerStateAthena->DeathInfo.DeathCause,
			false
		);

		// Now we need to calculate if the player or team won
		bool bTeamWon = FortGameStateAthena->TeamsLeft <= 1;
		if (bTeamWon && !PlayerStateAthena->bHasWonAGame) {
			AFortPlayerControllerAthena* WinnerPCAthena = KillerPCAthena;
			if (!WinnerPCAthena) {
				// Find the winning team and get a player controller from it
				for (AFortTeamInfo* Team : FortGameStateAthena->Teams) {
					for (AController* TeamMember : Team->TeamMembers) {
						AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
						if (TeamMemberController && TeamMemberController->bMarkedAlive) {
							WinnerPCAthena = TeamMemberController;
							break;
						}
					}
				}
			}
			if (!WinnerPCAthena) {
				// assume there was only 1 player playing this match
				WinnerPCAthena = this;
			}

			AFortPlayerStateAthena* WinnerPlayerStateAthena = WinnerPCAthena ? WinnerPCAthena->PlayerState->Cast<AFortPlayerStateAthena>() : nullptr;
			AFortPlayerPawnAthena* WinnerPlayerPawnAthena = WinnerPCAthena ? WinnerPCAthena->MyFortPawn->Cast<AFortPlayerPawnAthena>() : nullptr;

			if (WinnerPCAthena) {
				WinnerPCAthena->ClientNotifyWon(WinnerPlayerPawnAthena, FinishingWeapon, PlayerStateAthena->DeathInfo.DeathCause);

				if (WinnerPlayerStateAthena->PlayerTeam) {
					for (AController* TeamMember : WinnerPlayerStateAthena->PlayerTeam->TeamMembers) {
						AFortPlayerControllerAthena* TeamMemberController = TeamMember->Cast<AFortPlayerControllerAthena>();
						if (TeamMemberController) {
							AFortPlayerStateAthena* TeamMemberPlayerState = TeamMemberController->PlayerState->Cast<AFortPlayerStateAthena>();
							if (TeamMemberPlayerState) {
								TeamMemberPlayerState->Place = FortGameStateAthena->TeamsLeft; // we wanna do this before removing the player from alive players, so that the place is correct
								TeamMemberPlayerState->OnRep_Place();

								TeamMemberPlayerState->bHasWonAGame = true;
							}

							TeamMemberController->ClientNotifyTeamWon(WinnerPlayerPawnAthena, FinishingWeapon, PlayerStateAthena->DeathInfo.DeathCause);

							if (FAthenaRewardResult::StaticStruct()) {
								TeamMemberController->ClientSendEndBattleRoyaleMatchForPlayer(true, TeamMemberController->ConstructAthenaRewardResult());
							}

							if (FAthenaMatchStats::StaticStruct()) {
								TeamMemberController->ClientSendMatchStatsForPlayer(TeamMemberController->ConstructAthenaMatchStats());
							}

							if (FAthenaMatchTeamStats::StaticStruct()) {
								TeamMemberController->ClientSendTeamStatsForPlayer(TeamMemberController->ConstructAthenaMatchTeamStats());
							}
						}
					}
				}

				FortGameStateAthena->WinningTeam = WinnerPlayerStateAthena->TeamIndex;
				FortGameStateAthena->OnRep_WinningTeam();

				FortGameStateAthena->WinningPlayerName = WinnerPlayerStateAthena->GetPlayerName();
				FortGameStateAthena->OnRep_WinningPlayerName();

				FortGameStateAthena->WinningPlayerState = WinnerPlayerStateAthena;
				FortGameStateAthena->OnRep_WinningPlayerState();
			}
		}
	}

	if ((Version::Fortnite_Version <= 3.6 && Version::Fortnite_Version >= 1.91) || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
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
					if (PC && PC->Pawn) {
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

	TArray<FString> Medals;
	TArray<FFortQuestObjectiveCompletion> Advance;

	UFortQuestManager* QuestManager = GetQuestManager(ESubGame::GetAthena());
	if (QuestManager) {
		Advance = QuestManager->PendingChanges;
	}

	int32 MinutesAlive = PlayerStateAthena ? (PlayerStateAthena->SecondsAlive / 60) : -1;
	int32 PersonalKills = PlayerStateAthena ? PlayerStateAthena->KillScore : -1;
	int32 TeamKills = PlayerStateAthena ? PlayerStateAthena->TeamKillScore : -1;
	int32 Placement = PlayerStateAthena ? PlayerStateAthena->Place : -1;

	if (AthenaProfile) {
		FDedicatedServerUrlContext Context;
		AthenaProfile->EndBattleRoyaleGame(
			Advance,
			FortGameModeAthena->CurrentPlaylistId,
			MinutesAlive,
			PersonalKills,
			TeamKills,
			Placement,
			Medals,
			&Context
		);
	}

	Medals.Free();
	Advance.Free();
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
	
	FAthenaMatchStats* AthenaMatchStats = FAthenaMatchStats::Allocate();
	AthenaMatchStats->Place = PlayerStateAthena->Place;
	AthenaMatchStats->TotalPlayers = FortGameStateAthena->TotalPlayers;
	AthenaMatchStats->SecondsAlive = PlayerStateAthena->SecondsAlive;
	AthenaMatchStats->Kills = PlayerStateAthena->KillScore;
	AthenaMatchStats->Downs = PlayerStateAthena->DownScore;
	//AthenaMatchStats->Assists = (what do we even put here???) 
	//AthenaMatchStats->Revives = (what do we even put here???) 
	//AthenaMatchStats->DamageDealtToHostiles = (what do we even put here???) 
	//AthenaMatchStats->DamageDealtToFriends = (what do we even put here???) 
	//AthenaMatchStats->DamageDealtToStructures = (what do we even put here???) 
	//AthenaMatchStats->DamageTaken = (what do we even put here???) 
	//AthenaMatchStats->RangedHit = (what do we even put here???) 
	//AthenaMatchStats->RangedMiss = (what do we even put here???) 
	//AthenaMatchStats->Accuracy = (what do we even put here???) 
	//AthenaMatchStats->TravelDistanceGround = (what do we even put here???) 
	//AthenaMatchStats->MaterialsGathered = (what do we even put here???) 
	//AthenaMatchStats->MaterialsUsed = (what do we even put here???) 
	//AthenaMatchStats->CriticalShots = (what do we even put here???) 

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

	FAthenaMatchTeamStats* AthenaMatchTeamStats = FAthenaMatchTeamStats::Allocate();
	AthenaMatchTeamStats->Place = PlayerStateAthena->Place;
	AthenaMatchTeamStats->TotalPlayers = FortGameStateAthena->TotalPlayers;

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

	if (!QuickChatBank->ChatOptions.IsValidIndex(ChatEntry.Index)) {
		Log("ServerPlaySquadQuickChatMessage: ChatEntry index is out of bounds!");
		return;
	}

	FAthenaQuickChatLeafEntry& LeafEntry = QuickChatBank->ChatOptions.GetWithSize(ChatEntry.Index, FAthenaQuickChatLeafEntry::GetSize());

	ServerPlayEmoteItem(This, LeafEntry.EmojiItemDefinition, 0.f);

	PlayerStateAthena->TeamMemberState = LeafEntry.TeamCommType;
	PlayerStateAthena->ReplicatedTeamMemberState = LeafEntry.TeamCommType;
	PlayerStateAthena->OnRep_ReplicatedTeamMemberState();
}