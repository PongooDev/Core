#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Mcp/FortMcpProfile.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

struct FFortSubgameClientSettings {
public:
	DefineUnrealStruct(FFortSubgameClientSettings);

	DefineStructProperty(TArray<FString>, PinnedQuestInstances);
public:
	uint8 Padding[0x10];
};

class UFortMcpProfileSubgame : public UFortMcpProfile {
public:
	DefineUnrealClass(UFortMcpProfileSubgame);

	DefineUProperty(FFortSubgameClientSettings, CachedClientSettings);
};
