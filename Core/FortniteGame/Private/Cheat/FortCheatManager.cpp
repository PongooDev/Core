#include "pch.h"
#include "FortniteGame/Public/Cheat/FortCheatManager.h"

void UFortCheatManager::AddAllScores(int32 Amount)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"AddAllScores"));

	struct FortCheatManager_AddAllScores
	{
	public:
		int32 Amount;
	};

	FortCheatManager_AddAllScores Parms{};

	Parms.Amount = Amount;

	ProcessEvent(Func, &Parms);
}

void UFortCheatManager::AddBuildingScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddBuildingScore");

	struct FortCheatManager_AddBuildingScore final
	{
	public:
		int32 Amount;
	};

	FortCheatManager_AddBuildingScore Parms{};

	Parms.Amount = Amount;

	ProcessEvent(Func, &Parms);
}

void UFortCheatManager::AddCombatScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddCombatScore");

	struct FortCheatManager_AddCombatScore final
	{
	public:
		int32 Amount;
	};

	FortCheatManager_AddCombatScore Parms{};

	Parms.Amount = Amount;

	ProcessEvent(Func, &Parms);
}

void UFortCheatManager::AddUtilityScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddUtilityScore");

	struct FortCheatManager_AddUtilityScore final
	{
	public:
		int32 Amount;
	};

	FortCheatManager_AddUtilityScore Parms{};

	Parms.Amount = Amount;

	ProcessEvent(Func, &Parms);
}

void UFortCheatManager::AddKillFeedMessage()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddKillFeedMessage");

	ProcessEvent(Func, nullptr);
}