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
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"

bool AFortGameMode::SpawnPlayerBot(AActor* SpawnPoint)
{
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortGameMode::SpawnPlayerBot: World is null!");
		return false;
	}

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

		BotController->InitPlayerState();
		if (!BotController->PlayerState) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to initialize BotController's PlayerState!");
			return false;
		}

		BotController->PlayerState->bIsABot = true;

		APawn* BotPawn = (APawn*)World->SpawnActor(DefaultPawnClass.Class, SpawnLocation, SpawnRotation);
		if (!BotPawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to manually spawn bot pawn!");
			return false;
		}

		BotController->Possess(BotPawn);
		if (BotController->Pawn != BotPawn) {
			Log("AFortGameMode::SpawnPlayerBot: Failed to possess bot pawn!");
			return false;
		}

		AFortPlayerStateAthena* FortPSAthena = BotController->PlayerState->Cast<AFortPlayerStateAthena>();
		AFortPlayerControllerAthena* FortPCAthena = BotController->Cast<AFortPlayerControllerAthena>();
		AFortPlayerPawnAthena* FortPlayerPawnAthena = BotController->Pawn->Cast<AFortPlayerPawnAthena>();
		AFortGameModeAthena* FortGameModeAthena = Cast<AFortGameModeAthena>();

		if (FortPCAthena && FortPlayerPawnAthena) {
			if (FortGameModeAthena) {
				FortGameModeAthena->AddToAlivePlayers(FortPCAthena);
			}

			if (!FortPSAthena->HeroType) {
				if (FortPCAthena->DefaultHeroes.Num() > 0) {
					UFortHeroType* RandomHeroType = FortPCAthena->DefaultHeroes[UKismetMathLibrary::RandomIntegerInRange(0, FortPCAthena->DefaultHeroes.Num() - 1)];
					if (RandomHeroType) {
						FortPSAthena->HeroType = RandomHeroType;
						FortPSAthena->HeroId = RandomHeroType->GetName();
					}
				}
			}
			UFortHeroType* HeroType = FortPSAthena->HeroType;

			if (Version::Fortnite_Version <= 1.72) {
				if (HeroType && HeroType->GetSpecializationsAssetPtr().Num() > 0) {
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

				FortPSAthena->OnRep_CharacterParts();
			}
			else {
				// ApplyCharacterCustomization
			}
		}

		return true;
	}

	return false;
}