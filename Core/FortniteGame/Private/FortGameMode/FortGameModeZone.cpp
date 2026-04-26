#include "pch.h"
#include "FortniteGame/Public/FortGameMode/FortGameModeZone.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

void AFortGameModeZone::HandleStartingNewPlayer(AFortGameModeZone* This, AFortPlayerControllerZone* NewPlayer) {
	HandleStartingNewPlayerOG(This, NewPlayer);

	Log("HandleStartingNewPlayer Called!");
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("HandleStartingNewPlayer: World is null!");
		return;
	}

	auto PlayerState = (AFortPlayerStateZone*)NewPlayer->PlayerState;
	if (PlayerState) {
		if (PlayerState->AbilitySystemComponent) {
			UFortAbilitySet* FortAbilitySet = nullptr;

			if (Version::Fortnite_Version >= 2) {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_AthenaPlayer.GAS_AthenaPlayer");
			}
			else {
				FortAbilitySet = StaticLoadObject<UFortAbilitySet>("/Game/Abilities/Player/Generic/Traits/DefaultPlayer/GAS_DefaultPlayer.GAS_DefaultPlayer");
			}

			TScriptInterface<IAbilitySystemInterface> AbilitySystemInterface = TScriptInterface<IAbilitySystemInterface>();
			AbilitySystemInterface.ObjectPointer = PlayerState;
			AbilitySystemInterface.InterfacePointer = PlayerState->GetInterfaceAddress(IAbilitySystemInterface::StaticClass());

			UFortKismetLibrary::EquipFortAbilitySet(AbilitySystemInterface, FortAbilitySet, PlayerState);
		}
	}
}

void AFortGameModeZone::Hook() {
	HookVTable(AFortGameModeAthena::GetDefaultObj(), AGameModeBase::StaticClass()->GetFunction("Function /Script/Engine.GameModeBase.HandleStartingNewPlayer"), HandleStartingNewPlayer, (LPVOID*)&HandleStartingNewPlayerOG);

	Log("Hooked AFortGameModeAthena");
}