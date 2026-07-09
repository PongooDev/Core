#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameMode.h"

#include "FortniteGame/Public/AI/FortAIController.h"
#include "FortniteGame/Public/AI/FortAIPawn.h"
#include "FortniteGame/Public/FortPlayerStart/FortPlayerStartWarmup.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortCharacter/FortCharacter.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortInventory/FortQuickBars.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeFrontEnd.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortGameSession/FortGameSessionDedicated.h"
#include "FortniteGame/Public/FortManager/FortMissionManager.h"
#include "FortniteGame/Public/FortManager/FortFeedbackManager.h"
#include "FortniteGame/Public/FortManager/FortBluGloManager.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"

APawn* AFortGameMode::SpawnDefaultPawnFor(AFortGameMode* This, AController* NewPlayer, AActor* StartSpot) {
	return SpawnDefaultPawnForOG(This, NewPlayer, StartSpot);
}

bool AFortGameMode::SpawnPlayerBot(AActor* SpawnPoint)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameMode::SpawnPlayerBot: World is null!");
		return false;
	}

	AFortGameMode* FortGameMode = this->Cast<AFortGameMode>();
	AFortGameModeZone* FortGameModeZone = FortGameMode ? FortGameMode->Cast<AFortGameModeZone>() : nullptr;
	AFortGameModeAthena* FortGameModeAthena = this->Cast<AFortGameModeAthena>();

	static TArray<AActor*> PlayerStarts;
	if (PlayerStarts.Num() == 0) {
		UGameplayStatics::GetAllActorsOfClass(World, AFortPlayerStartWarmup::StaticClass(), &PlayerStarts);
	}
	
	int32 MaxSpawnLocations = PlayerStarts.Num();
	static int32 BotSpawnLocationIndex = 0;

	if (Version::Fortnite_Version <= 1.72) {
		if (!SpawnPoint && PlayerStarts.Num() > 0) {
			if (BotSpawnLocationIndex >= MaxSpawnLocations) {
				BotSpawnLocationIndex = 0;
			}
			SpawnPoint = PlayerStarts.IsValidIndex(BotSpawnLocationIndex) ? PlayerStarts[BotSpawnLocationIndex] : nullptr;
			BotSpawnLocationIndex++;
		}

		if (!SpawnPoint) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to find spawn point!");
			return false;
		}

		FVector SpawnLocation = SpawnPoint->K2_GetActorLocation();
		FRotator SpawnRotation = SpawnPoint->K2_GetActorRotation();

		APlayerController* BotController = SpawnPlayerController(ROLE_SimulatedProxy, SpawnLocation, SpawnRotation);
		if (!BotController) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to spawn BotController!");
			return false;
		}

		AFortPlayerController* FortPC = BotController->Cast<AFortPlayerController>();
		AFortPlayerControllerAthena* FortPCAthena = BotController->Cast<AFortPlayerControllerAthena>();

		BotController->InitPlayerState();
		if (!BotController->PlayerState) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to initialize BotController's PlayerState!");
			return false;
		}

		AFortPlayerState* FortPS = BotController->PlayerState->Cast<AFortPlayerState>();
		AFortPlayerStateAthena* FortPSAthena = BotController->PlayerState->Cast<AFortPlayerStateAthena>();

		BotController->PlayerState->bIsABot = true;

		if (FortGameModeAthena) {
			FortGameModeAthena->AddToAlivePlayers(FortPCAthena);
			NumBots++;
		}

		RestartPlayerAtTransform(BotController, SpawnPoint->GetTransform());
		if (!BotController->Pawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to spawn bot pawn!");
			return false;
		}

		AFortPawn* FortPawn = BotController->Pawn->Cast<AFortPawn>();
		AFortPlayerPawn* FortPlayerPawn = BotController->Pawn->Cast<AFortPlayerPawn>();
		AFortPlayerPawnAthena* FortPlayerPawnAthena = BotController->Pawn->Cast<AFortPlayerPawnAthena>();

		if (FortPawn) {
			FortPawn->SetMaxHealth(100.0f);
			FortPawn->SetHealth(100.0f);
			FortPawn->SetMaxShield(100.0f);
			FortPawn->SetShield(0.0f);
		}

		if (FortPC && FortPlayerPawn) {
			if (!FortPC->MyFortPawn) {
				FortPC->MyFortPawn = FortPlayerPawn;
			}
		}
		
		if (FortPCAthena) {
			if (FortPS) {
				if (!FortPS->HeroType && FortPCAthena->DefaultHeroes.Num() > 0) {
					UFortHeroType* RandomHeroType = FortPCAthena->DefaultHeroes[UKismetMathLibrary::RandomIntegerInRange(0, FortPCAthena->DefaultHeroes.Num() - 1)];
					if (RandomHeroType) {
						FortPS->HeroType = RandomHeroType;
						FortPS->HeroId = RandomHeroType->GetName();
					}
				}

				UFortHeroType* HeroType = FortPS->HeroType;

				if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.1 && Version::Fortnite_Version != 1.11) {
					if (FortPlayerPawn && HeroType && HeroType->GetSpecializationsAssetPtr().Num() > 0) {
						UFortHeroSpecialization* RandomSpecialization = HeroType->GetSpecializationsAssetPtr()[UKismetMathLibrary::RandomIntegerInRange(0, HeroType->GetSpecializationsAssetPtr().Num() - 1)].Get();
						if (RandomSpecialization) {
							int32 NumParts = RandomSpecialization->GetCharacterPartsAssetPtr().Num();

							for (int32 i = 0; i < NumParts; i++) {
								TAssetPtr<UCustomCharacterPart>& PartPtr = RandomSpecialization->GetCharacterPartsAssetPtr()[i];

								UCustomCharacterPart* CharacterPartAsset = PartPtr.Get();

								if (CharacterPartAsset) {
									FortPlayerPawnAthena->ServerChoosePart(CharacterPartAsset, CharacterPartAsset->CharacterPartType);
								}
								else {
									Log("AFortGameMode::SpawnPlayerBot: Failed to load character part asset for part index: " + std::to_string(i));
								}
							}
						}
						else {
							Log("AFortGameMode::SpawnPlayerBot: Failed to get random specialization!");
						}
					}

					FortPS->OnRep_CharacterParts();
				}
				else {
					FortPS->ApplyCharacterCustomization(FortPlayerPawn);
				}
			}
		}

		return true;
	}

	return false;
}

uint8 AFortGameMode::PickTeam(uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor) {
	uint8 (*&PickTeamInternal)(AFortGameMode* This, uint8 PreferredTeam, AFortPlayerController* ControllerToPickFor) = decltype(PickTeamInternal)(VTable[Finder::FindAFortGameMode_PickTeamVFT()]);
	return PickTeamInternal(this, PreferredTeam, ControllerToPickFor);
}

UClass** AFortGameMode::GetGameSessionClass(AFortGameMode* This, UClass** result) {
	Log("AFortGameMode::GetGameSessionClass called.");

	if (ConfigurationManager::GetConfig().bUseGameSessions) {
		*result = AFortGameSessionDedicated::StaticClass();
		This->GameSessionClass = *result;

		return result;
	}
	else {
		return GetGameSessionClassOG(This, result);
	}
}

void AFortGameMode::RestartPlayerHK(AFortGameMode* This, AController* NewPlayer) {
	RestartPlayerOG(This, NewPlayer);
	if (Version::Fortnite_Version > 2.5) {
		return;
	}

	AFortPlayerController* FortPC = NewPlayer->Cast<AFortPlayerController>();
	if (FortPC) {
		if (FortPC->WorldInventory) {
			if (FortPC->IsUsingOldQuickBars()) {
				FortPC->WorldInventory->EquipHarvestingTool();
			}
		}
	}
}

void AFortGameMode::FinishWorldInitialization(AFortGameMode* This, AFortWorldManager* WorldManager) {
	FinishWorldInitializationOG(This, WorldManager);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameMode::FinishWorldInitialization: World is null!");
		return;
	}

	AFortGameState* FortGameState = This->GameState->Cast<AFortGameState>();
	if (!FortGameState) {
		Log("AFortGameMode::FinishWorldInitialization: GameState is null or not AFortGameState!");
		return;
	}

	if (This->MissionManagerClass.Get() && !FortGameState->MissionManager) {
		Log("MissionManagerClass: " + This->MissionManagerClass.Get()->GetName().ToString());
		AFortMissionManager* MissionManager = World->SpawnActor(
			This->MissionManagerClass.Class,
			FVector(),
			FRotator(),
			FortGameState
		)->Cast<AFortMissionManager>();
		if (MissionManager) {
			FortGameState->MissionManager = MissionManager;
			FortGameState->OnRep_MissionManager();
		}
	}

	Log("MissionManager: " + (FortGameState->MissionManager ? FortGameState->MissionManager->GetName().ToString() : "None"));

	This->bWorldIsReady = true;
}

void AFortGameMode::AddInactivePlayerHK(AFortGameMode* This, APlayerState* PlayerState, APlayerController* PC) {
	return AGameMode::AddInactivePlayerOG(This, PlayerState, PC);
}

bool AFortGameMode::ReadyToStartMatch(AFortGameMode* This) {
	return ReadyToStartMatchOG(This);
}

void AFortGameMode::PreInitializeComponents(AFortGameMode* This) {
	PreInitializeComponentsOG(This);
}

void AFortGameMode::InitializeTeams() {
	void (*&InitializeTeamsInternal)(AFortGameMode* This) = decltype(InitializeTeamsInternal)(VTable[Finder::FindAFortGameMode_InitializeTeamsVFT()]);
	InitializeTeamsInternal(this);
}