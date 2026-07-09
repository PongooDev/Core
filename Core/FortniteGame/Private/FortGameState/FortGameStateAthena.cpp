#include "pch.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"

#include "FortniteGame/Public/FortHero/FortHero.h"
#include "FortniteGame/Public/FortHero/FortHeroSpecialization.h"
#include "FortniteGame/Public/FortHero/FortHeroType.h"

void AFortGameStateAthena::OnRep_CurrentPlaylistId()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"OnRep_CurrentPlaylistId"));

	Call(Func);
}

void AFortGameStateAthena::OnPlaylistDataLoadCompleted() {
	void (*OnPlaylistDataLoadCompletedInternal)(AFortGameStateAthena*) = decltype(OnPlaylistDataLoadCompletedInternal)(ImageBase + Finder::FindAFortGameStateAthena_OnPlaylistDataLoadCompleted());
	OnPlaylistDataLoadCompletedInternal(this);
}

void AFortGameStateAthena::UpdatePlaylistDependentData() {
	void (*UpdatePlaylistDependentDataInternal)(AFortGameStateAthena*) = decltype(UpdatePlaylistDependentDataInternal)(ImageBase + Finder::FindAFortGameStateAthena_UpdatePlaylistDependentData());
	UpdatePlaylistDependentDataInternal(this);
}

void AFortGameStateAthena::SetCurrentPlaylistId(int InPlaylistId) {
	if (Finder::FindAFortGameStateAthena_SetCurrentPlaylistId()) {
		void (*SetCurrentPlaylistIdInternal)(AFortGameStateAthena* This, int InPlaylistId) = decltype(SetCurrentPlaylistIdInternal)(ImageBase + Finder::FindAFortGameStateAthena_SetCurrentPlaylistId());
		SetCurrentPlaylistIdInternal(this, InPlaylistId);
	}
	else {
		CurrentPlaylistId = InPlaylistId;
		OnRep_CurrentPlaylistId();
	}
}

void AFortGameStateAthena::ApplyHomebaseEffectsOnPlayerSetup(AFortGameStateAthena* This, FUniqueNetIdRepl* SourceAccountID, UFortMcpProfileCampaign* McpProfile, IAbilitySystemInterface* AbilityObject, UFortHero* Hero, bool bApplyTeamEffect, bool bApplyTeamEffectToOtherPlayers, bool bIgnoreStatClamp)
{
	if (Version::Fortnite_Version > 2.42) {
		return ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
	}

	UFortHeroType* ItemDefinition = StaticLoadObject<UFortHeroType>("/Game/Athena/Heroes/HID_Commando_Athena_01.HID_Commando_Athena_01");

	static TArray<UObject*> HeroTypes = FUObjectArray::GetObjectsOfClass(UFortHeroType::StaticClass(), "Athena");
	if (HeroTypes.Num() > 0)
	{
		ItemDefinition = (UFortHeroType*)HeroTypes[UKismetMathLibrary::RandomIntegerInRange(0, HeroTypes.Num() - 1)];
	}

	if (ItemDefinition)
	{
		Hero->ItemDefinition = ItemDefinition;
		Log("AFortGameStateAthena::ApplyHomebaseEffectsOnPlayerSetup: Set hero item definition to " + ItemDefinition->GetName().ToString());
	}

	ApplyHomebaseEffectsOnPlayerSetupOG(This, SourceAccountID, McpProfile, AbilityObject, Hero, bApplyTeamEffect, bApplyTeamEffectToOtherPlayers, bIgnoreStatClamp);
}

void AFortGameStateAthena::OnRep_WinningPlayerName()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerName");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningTeam()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningTeam");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningPlayerList()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerList");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_WinningPlayerState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_WinningPlayerState");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortGameStateAthena::OnRep_CurrentPlaylistData()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentPlaylistData");

	Call(Func);
}

void AFortGameStateAthena::OnRep_CurrentPlaylistInfo()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentPlaylistInfo");

	Call(Func);
}

void AFortGameStateAthena::Hook() {
	HookEveryVTableIdx(
		AFortGameStateAthena::StaticClass(),
		Finder::FindAFortGameState_ApplyHomebaseEffectsOnPlayerSetupVFT(),
		ApplyHomebaseEffectsOnPlayerSetup,
		(LPVOID*)&ApplyHomebaseEffectsOnPlayerSetupOG
	);

	Log("Hooked AFortGameStateAthena");
}