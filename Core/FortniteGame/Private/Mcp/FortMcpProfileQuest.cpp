#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileQuest.h"

#include "FortniteGame/Public/Mcp/McpProfileSys.h"

void UFortMcpProfileQuest::ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerQuestLogin");

	struct FortMcpProfileQuest_ServerQuestLogin
	{
	public:
		FString MatchmakingSessionId;
		FDedicatedServerUrlContext Context;
	};

	FortMcpProfileQuest_ServerQuestLogin Parms{};

	Parms.MatchmakingSessionId = std::move(MatchmakingSessionId);

	ProcessEvent(Func, &Parms);

	if (Context != nullptr)
		*Context = std::move(Parms.Context);
}

void UFortMcpProfileQuest::UpdateQuests(const TArray<FFortQuestObjectiveCompletion>& Advance, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("UpdateQuests");

	struct FortMcpProfileQuest_UpdateQuests
	{
	public:
		TArray<FFortQuestObjectiveCompletion> Advance;
		FDedicatedServerUrlContext Context;
	};

	FortMcpProfileQuest_UpdateQuests Parms{};

	Parms.Advance = std::move(Advance);

	ProcessEvent(Func, &Parms);

	if (Context != nullptr)
		*Context = std::move(Parms.Context);
}