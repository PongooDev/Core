#pragma once
#include "pch.h"

#include "FortniteGame/Public/Mcp/FortMcpProfileQuest.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"

struct FDedicatedServerUrlContext;

class UFortMcpProfileAthena : public UFortMcpProfileQuest {
public:
	DefineUnrealClass(UFortMcpProfileAthena);
public:
	void EndBattleRoyaleGame(
		const TArray<FFortQuestObjectiveCompletion>& Advance,
		uint8 MatchType,
		int32 MinutesAlive,
		int32 PersonalKills,
		int32 TeamKills,
		int32 Placement,
		const TArray<FString>& Medals,
		FDedicatedServerUrlContext* Context
	);

	void EndBattleRoyaleGame(
		const TArray<FFortQuestObjectiveCompletion>& Advance,
		const FString& PlaylistId,
		const FAthenaMatchStats& MatchStats,
		int32 FriendshipXpBoost,
		int32 CosmeticXpBoost,
		float AntiAddictionPlayTimeMultiplier,
		bool ShouldAccumulateToProfileStats,
		bool ShouldSaveToRecentGameLists,
		const TArray<FString>& Medals,
		FDedicatedServerUrlContext* Context
	);
};