#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Disco.h"

#include "FortniteGame/Public/Capture/AthenaCapturePoint.h"

bool AFortAthenaMutator_Disco::IsRespawningAllowed()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("IsRespawningAllowed");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::IsRespawningAllowed: Failed to find function!");
		return false;
	}

	return Call<bool>(Func);
}

void AFortAthenaMutator_Disco::OnCaptureLock(AAthenaCapturePoint* CapturePoint, uint8 LockTeam, bool bIsNewLock)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnCaptureLock");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::OnCaptureLock: Failed to find function!");
		return;
	}

	return Call<void>(Func, CapturePoint, LockTeam, bIsNewLock);
}

void AFortAthenaMutator_Disco::OnCaptureLockBroken(AAthenaCapturePoint* CapturePoint, uint8 LockTeam, uint8 BreakTeam)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnCaptureLockBroken");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::OnCaptureLockBroken: Failed to find function!");
		return;
	}

	return Call<void>(Func, CapturePoint, LockTeam, BreakTeam);
}

void AFortAthenaMutator_Disco::OnGamePhaseChanged(uint8 GamePhase)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGamePhaseChanged");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::OnGamePhaseChanged: Failed to find function!");
		return;
	}

	return Call<void>(Func, GamePhase);
}

void AFortAthenaMutator_Disco::OnGamePhaseStepChanged(uint8 GamePhaseStep)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGamePhaseStepChanged");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::OnGamePhaseStepChanged: Failed to find function!");
		return;
	}

	return Call<void>(Func, GamePhaseStep);
}

void AFortAthenaMutator_Disco::OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2, int32 EventParam3)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnMutatorGameplayEvent");

	if (!Func) {
		Log("AFortAthenaMutator_Disco::OnMutatorGameplayEvent: Failed to find function!");
		return;
	}

	return Call<void>(Func, EventId, EventParam1, EventParam2, EventParam3);
}
