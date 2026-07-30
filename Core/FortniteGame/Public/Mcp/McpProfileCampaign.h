#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Mcp/McpProfileSubgame.h"

class UFortMcpProfileCampaign : public UFortMcpProfileSubgame {
public:
	DefineUnrealClass(UFortMcpProfileCampaign);

	DefineUProperty(bool, bEnableHeroLoadoutMocking);
	DefineUProperty(float, HeroLoadoutCommandDelaySeconds);
};
