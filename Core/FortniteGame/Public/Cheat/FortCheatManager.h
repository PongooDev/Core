#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/CheatManager.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

class UFortCheatManager : public UCheatManager {
public:
	DefineUnrealClass(UFortCheatManager);
public:
	void AddAllScores(int32 Amount);

	void AddBuildingScore(int32 Amount);

	void AddCombatScore(int32 Amount);

	void AddUtilityScore(int32 Amount);

	void AddKillFeedMessage();
};