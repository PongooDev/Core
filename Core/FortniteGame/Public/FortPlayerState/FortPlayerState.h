#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerState.h"

class UFortAbilitySystemComponent;
class AFortPlayerPawn;
class UFortHeroType;

class AFortPlayerState : public APlayerState {
public:
	DefineUnrealClass(AFortPlayerState);

	DefineUProperty(UFortAbilitySystemComponent*, AbilitySystemComponent);
	DefineBitfieldUProperty(bHasFinishedLoading);
	DefineBitfieldUProperty(bHasStartedPlaying);
	DefineUProperty(FString, HeroId);
	DefineUProperty(UFortHeroType*, HeroType);
	DefineUProperty(FName, HeroSquadId);
	DefineUProperty(int32, WorldPlayerId);
public:
	void OnRep_bHasStartedPlaying();

	void OnRep_CharacterParts();

	void OnRep_HeroType();

	void InitializeHero();

	void ApplyCharacterCustomization(AFortPlayerPawn* FortPlayerPawn);
};