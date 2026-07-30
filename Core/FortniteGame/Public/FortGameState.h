#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/GameState.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"

#include "FortniteGame/Public/World/AdditionalLevelStreamed.h"

class FUniqueNetIdRepl;
class UFortMcpProfileCampaign;
class UFortHero;
class IAbilitySystemInterface;
class AFortMissionManager;
class AFortFeedbackManager;
class AFortTeamInfo;
class AFortWorldManager;

class AFortGameState : public AGameState {
public:
	DefineUnrealClass(AFortGameState);

	DefineUProperty(TArray<FAdditionalLevelStreamed>, AdditionalPlaylistLevelsStreamed);
	DefineUProperty(AFortFeedbackManager*, FeedbackManager);
	DefineUProperty(AFortMissionManager*, MissionManager);
	DefineUProperty(AFortWorldManager*, WorldManager);
	DefineUProperty(FString, GameSessionID);
	DefineUProperty(int32, TeamCount);
	DefineUProperty(int32, TeamSize);
	DefineUProperty(TArray<AFortTeamInfo*>, Teams);
	DefineBitfieldUProperty(bDBNOEnabledForGameMode);
	DefineUProperty(int32, WorldLevel);
public:
	void OnRep_AdditionalPlaylistLevelsStreamed();
	void OnFinishedStreamingAdditionalPlaylistLevel();
	void OnRep_MissionManager();

	static void Hook();
};