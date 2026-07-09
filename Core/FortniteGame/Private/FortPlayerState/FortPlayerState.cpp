#include "pch.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerState.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortCharacter/CustomCharacterPart.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"

void AFortPlayerState::OnRep_bHasStartedPlaying()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_bHasStartedPlaying"));

	Call(Func);
}

void AFortPlayerState::OnRep_CharacterParts()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CharacterParts"));

	Call(Func);
}

void AFortPlayerState::OnRep_HeroType()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_HeroType");

	Call(Func);
}

void AFortPlayerState::InitializeHero() {
	void (*InitializeHeroInternal)(AFortPlayerState*) = decltype(InitializeHeroInternal)(ImageBase + Finder::FindAFortPlayerState_InitializeHero());
	InitializeHeroInternal(this);
}

void AFortPlayerState::ApplyCharacterCustomization(AFortPlayerPawn* FortPlayerPawn) {
	if (!FortPlayerPawn) {
		Log("AFortPlayerState::ApplyCharacterCustomization: FortPlayerPawn is null for PlayerState " + GetName().ToString());
		return;
	}

	if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.1 && Version::Fortnite_Version != 1.11) {
		AFortPlayerControllerAthena* FortPCAthena = FortPlayerPawn->Controller->Cast<AFortPlayerControllerAthena>();
		
		UCustomCharacterPart* HeadPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Heads/F_Med_Head1.F_Med_Head1");
		UCustomCharacterPart* BodyPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Female/Medium/Bodies/F_Med_Soldier_01.F_Med_Soldier_01");
		UCustomCharacterPart* BackpackPart = (UCustomCharacterPart*)StaticLoadObject("/Game/Characters/CharacterParts/Backpacks/NoBackpack.NoBackpack");

		if (HeadPart) {
			FortPlayerPawn->ServerChoosePart(HeadPart, HeadPart->CharacterPartType);
		}
		if (BodyPart) {
			FortPlayerPawn->ServerChoosePart(BodyPart, BodyPart->CharacterPartType);
		}
		if (BackpackPart) {
			FortPlayerPawn->ServerChoosePart(BackpackPart, BackpackPart->CharacterPartType);
		}
		
		if (FortPCAthena) {
			if (FortPCAthena->StrongMyHero && FortPCAthena->StrongMyHero->CharacterParts.Num() > 0) {
				for (UCustomCharacterPart* CharacterPart : FortPCAthena->StrongMyHero->CharacterParts) {
					FortPlayerPawn->ServerChoosePart(CharacterPart, CharacterPart->CharacterPartType);
				}
			}
			else {
				Log("AFortPlayerState::ApplyCharacterCustomization: StrongMyHero or CharacterParts is null or empty for PlayerState " + GetName().ToString());
			}
		}
		else {
			Log("AFortPlayerState::ApplyCharacterCustomization: FortPCAthena is null for PlayerState " + GetName().ToString());
		}

		OnRep_CharacterParts();
	}
	else {
		void (*ApplyCharacterCustomizationInternal)(AFortPlayerState*, AFortPlayerPawn*) = decltype(ApplyCharacterCustomizationInternal)(ImageBase + Finder::FindAFortPlayerState_ApplyCharacterCustomization());
		ApplyCharacterCustomizationInternal(this, FortPlayerPawn);
	}
}