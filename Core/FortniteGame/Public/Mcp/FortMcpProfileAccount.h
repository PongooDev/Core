#pragma once
#include "pch.h"

#include "FortniteGame/Public/Mcp/FortMcpProfile.h"

struct FDedicatedServerUrlContext;

class UFortMcpProfileAccount : public UFortMcpProfile {
public:
	DefineUnrealClass(UFortMcpProfileAccount);
public:
	void ServerQuestLogin(const FString& MatchmakingSessionId, FDedicatedServerUrlContext* Context);
};