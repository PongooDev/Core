#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAthena.h"

#include "FortniteGame/Public/Mcp/McpProfileSys.h"

void UFortMcpProfileAthena::EndBattleRoyaleGame(const TArray<FFortQuestObjectiveCompletion>& Advance, uint8 MatchType, int32 MinutesAlive, int32 PersonalKills, int32 TeamKills, int32 Placement, const TArray<FString>& Medals, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("EndBattleRoyaleGame");

	return Call(Func, Advance, MatchType, MinutesAlive, PersonalKills, TeamKills, Placement, Medals, Context);
}