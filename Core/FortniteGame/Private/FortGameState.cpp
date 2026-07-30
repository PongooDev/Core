#include "pch.h"
#include "FortniteGame/Public/FortGameState.h"

#include "FortniteGame/Public/Heroes/FortHeroType.h"
#include "FortniteGame/Public/Heroes/FortHero.h"

void AFortGameState::OnRep_AdditionalPlaylistLevelsStreamed()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_AdditionalPlaylistLevelsStreamed"));

	Call(Func);
}

void AFortGameState::OnFinishedStreamingAdditionalPlaylistLevel()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnFinishedStreamingAdditionalPlaylistLevel"));

	Call(Func);
}

void AFortGameState::OnRep_MissionManager()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_MissionManager");

	Call(Func);
}

void AFortGameState::Hook()
{
	Log("Hooked AFortGameState");
}