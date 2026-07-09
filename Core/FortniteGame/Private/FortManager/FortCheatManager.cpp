#include "pch.h"
#include "FortniteGame/Public/FortManager/FortCheatManager.h"

void UFortCheatManager::AddAllScores(int32 Amount)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"AddAllScores"));

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddBuildingScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddBuildingScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddCombatScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddCombatScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddUtilityScore(int32 Amount)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddUtilityScore");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func, Amount);
}

void UFortCheatManager::AddKillFeedMessage()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("AddKillFeedMessage");

	return const_cast<UFortCheatManager*>(this)->Call<void>(Func);
}