#pragma once
#include "pch.h"

#include "FortMcpProfile.h"

#include "FortniteGame/Public/FortQuest/FortQuestObjectiveCompletion.h"

struct FDedicatedServerUrlContext;
struct FFortQuestObjectiveCompletion;

class UFortMcpProfileQuest : public UFortMcpProfile {
public:
	DefineUnrealClass(UFortMcpProfileQuest);
public:
	void ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context);
	void UpdateQuests(const TArray<FFortQuestObjectiveCompletion>& Advance, FDedicatedServerUrlContext* Context);
};