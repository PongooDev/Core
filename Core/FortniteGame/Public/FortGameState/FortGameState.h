#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameState.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "FortniteGame/Public/FortWorld/AdditionalLevelStreamed.h"

class FUniqueNetIdRepl;
class UFortMcpProfileCampaign;
class UFortHero;
class IAbilitySystemInterface;
class AFortMissionManager;
class AFortFeedbackManager;

class AFortGameState : public AGameState {
public:
	DefineUnrealClass(AFortGameState);

	DefineUProperty(TArray<FAdditionalLevelStreamed>, AdditionalPlaylistLevelsStreamed);
	DefineUProperty(AFortFeedbackManager*, FeedbackManager);
	DefineUProperty(AFortMissionManager*, MissionManager);
	DefineUProperty(FString, GameSessionID);
public:
	void OnRep_AdditionalPlaylistLevelsStreamed();
	void OnFinishedStreamingAdditionalPlaylistLevel();
	void OnRep_MissionManager();

	static void Hook();
};