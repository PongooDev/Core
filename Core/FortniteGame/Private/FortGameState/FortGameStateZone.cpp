#include "pch.h"
#include "FortniteGame/Public/FortGameState/FortGameStateZone.h"

#include "FortniteGame/Public/FortHero/FortHeroType.h"
#include "FortniteGame/Public/FortHero/FortHero.h"

void AFortGameStateZone::OnRep_GameDifficulty()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_GameDifficulty");

	Call(Func);
}

void AFortGameStateZone::Hook() {
	Log("Hooked AFortGameStateZone");
}