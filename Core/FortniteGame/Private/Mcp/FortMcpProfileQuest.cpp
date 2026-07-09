#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileQuest.h"

#include "FortniteGame/Public/Mcp/McpProfileSys.h"

void UFortMcpProfileQuest::ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerQuestLogin");

	return Call(Func, MatchmakingSessionId, Context);
}

void UFortMcpProfileQuest::UpdateQuests(const TArray<FFortQuestObjectiveCompletion>& Advance, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("UpdateQuests");

	return Call(Func, Advance, Context);
}