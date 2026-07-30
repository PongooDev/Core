#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

#include "FortniteGame/Public/Athena/AthenaLevelInfo.h"
#include "FortniteGame/Public/Athena/AthenaMatchStats.h"
#include "FortniteGame/Public/Athena/AthenaMatchTeamStats.h"
#include "FortniteGame/Public/Athena/AthenaRewardResult.h"

class UAthenaPlayerMatchReport : public UObject {
public:
	DefineUnrealClass(UAthenaPlayerMatchReport);

	DefineUProperty(FAthenaLevelInfo, InitialLevelInfo);
	DefineUProperty(bool, bHasMatchStats);
	DefineUProperty(FAthenaMatchStats, MatchStats);
	DefineUProperty(bool, bHasTeamStats);
	DefineUProperty(FAthenaMatchTeamStats, TeamStats);
	DefineUProperty(bool, bHasRewards);
	DefineUProperty(FAthenaRewardResult, EndOfMatchResults);
	DefineUProperty(FAthenaRewardResult, Rewards);
};