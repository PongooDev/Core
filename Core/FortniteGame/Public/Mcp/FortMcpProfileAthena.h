#pragma once
#include "pch.h"

#include "FortMcpProfileQuest.h"

struct FDedicatedServerUrlContext;

class UFortMcpProfileAthena : public UFortMcpProfileQuest {
public:
	DefineUnrealClass(UFortMcpProfileAthena);
public:
	void EndBattleRoyaleGame(const TArray<FFortQuestObjectiveCompletion>& Advance, uint8 MatchType, int32 MinutesAlive, int32 PersonalKills, int32 TeamKills, int32 Placement, const TArray<FString>& Medals, FDedicatedServerUrlContext* Context);
};