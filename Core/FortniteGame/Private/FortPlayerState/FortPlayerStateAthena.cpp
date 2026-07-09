#include "pch.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateAthena.h"

void AFortPlayerStateAthena::OnRep_TeamKillScore()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamKillScore");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_TeamScore()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamScore");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::ClientReportDBNO(const AFortPlayerStateAthena* Player)
{
	if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		ClientReportDBNO(Player->GetPlayerName());
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("ClientReportDBNO");

		if (!Func) {
			return;
		}

		return Call(Func, Player);
	}
}

void AFortPlayerStateAthena::ClientReportDBNO(const FString& DBNOPlayersName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportDBNO");

	if (!Func) {
		return;
	}

	return Call(Func, DBNOPlayersName);
}

void AFortPlayerStateAthena::ClientReportKill(const AFortPlayerStateAthena* Player)
{
	if (Version::Fortnite_Version <= 1.91 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		ClientReportKill(Player->GetPlayerName());
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = FindFunction("ClientReportKill");

		if (!Func) {
			return;
		}

		return Call(Func, Player);
	}
}

void AFortPlayerStateAthena::ClientReportKill(const FString& KilledPlayersName)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportKill");

	if (!Func) {
		return;
	}

	return Call(Func, KilledPlayersName);
}

void AFortPlayerStateAthena::ClientReportTeamKill(int32 TeamKills)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ClientReportTeamKill");

	if (!Func) {
		return;
	}

	return Call(Func, TeamKills);
}

void AFortPlayerStateAthena::OnRep_Kills()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Kills");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_DeathInfo()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_DeathInfo");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_Downs()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Downs");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_Place()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Place");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_TeamIndex()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_TeamIndex");

	if (!Func) {
		return;
	}

	Call(Func);
}

uint8 AFortPlayerStateAthena::ToDeathCause(const FGameplayTagContainer& InTags, bool bWasDBNO)
{
	if (Version::Fortnite_Version <= 3.6 || Version::Fortnite_Version == 1.10 || Version::Fortnite_Version == 1.11) {
		// need to reimplement this for older versions, but for now just return unspecified
		return EDeathCause::GetUnspecified();
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerStateAthena:ToDeathCause");

		return GetDefaultObj()->Call<uint8>(Func, InTags, bWasDBNO);
	}
}

void AFortPlayerStateAthena::OnRep_ReplicatedTeamMemberState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_ReplicatedTeamMemberState");

	if (!Func) {
		return;
	}

	Call(Func);
}

void AFortPlayerStateAthena::OnRep_SquadId()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_SquadId");

	if (!Func) {
		return;
	}

	Call(Func);
}