#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameState.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "FortniteGame/Public/FortWorld/AdditionalLevelStreamed.h"

class FUniqueNetIdRepl;
class UFortMcpProfileCampaign;
class UFortHero;
class IAbilitySystemInterface;

class AFortGameState : public AGameState {
public:
	DefineUnrealClass(AFortGameState);

	DefineUProperty(TArray<FAdditionalLevelStreamed>, AdditionalPlaylistLevelsStreamed);
public:
	void OnRep_AdditionalPlaylistLevelsStreamed();
	void OnFinishedStreamingAdditionalPlaylistLevel();

	static inline void (*ApplyHomebaseEffectsOnPlayerSetupOG)(AFortGameState* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp) = nullptr;
	static void ApplyHomebaseEffectsOnPlayerSetup(AFortGameState* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp);

	static void Hook();
};