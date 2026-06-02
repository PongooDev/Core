#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"

void UFortMcpProfileAccount::ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerQuestLogin");

	struct FortMcpProfileAccount_ServerQuestLogin
	{
	public:
		FString MatchmakingSessionId;
		FDedicatedServerUrlContext Context;
	};

	FortMcpProfileAccount_ServerQuestLogin Parms{};

	Parms.MatchmakingSessionId = std::move(MatchmakingSessionId);

	ProcessEvent(Func, &Parms);

	if (Context != nullptr)
		*Context = std::move(Parms.Context);
}