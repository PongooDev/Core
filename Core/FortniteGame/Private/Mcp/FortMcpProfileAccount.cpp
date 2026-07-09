#include "pch.h"
#include "FortniteGame/Public/Mcp/FortMcpProfileAccount.h"

#include "FortniteGame/Public/Mcp/McpProfileSys.h"

void UFortMcpProfileAccount::ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context)
{
	if (!this) return;

	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerQuestLogin");

	if (!Func) {
		Log("UFortMcpProfileAccount::ServerQuestLogin: Failed to find function!");
		return;
	}

	return Call(Func, MatchmakingSessionId, Context);
}