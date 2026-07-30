#include "pch.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_Heist.h"

#include "FortniteGame/Public/Athena/FortAthenaExitCraft.h"
#include "FortniteGame/Public/Athena/FortAthenaExitCraftSpawner.h"

void AFortAthenaMutator_Heist::OnGamePhaseChanged(uint8 GamePhase)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGamePhaseChanged");

	if (!Func) {
		return;
	}

	Call(Func, GamePhase);
}

void AFortAthenaMutator_Heist::OnGamePhaseStepChanged(uint8 GamePhaseStep)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnGamePhaseStepChanged");

	if (!Func) {
		return;
	}

	Call(Func, GamePhaseStep);
}

void AFortAthenaMutator_Heist::OnExitCraftSpawned(AFortAthenaExitCraft* ExitCraft, AFortAthenaExitCraftSpawner* ExitCraftSpawner)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnExitCraftSpawned");

	if (!Func) {
		return;
	}

	Call(Func, ExitCraft, ExitCraftSpawner);
}

void AFortAthenaMutator_Heist::OnExitCraftIsDestroying(AFortAthenaExitCraft* ExitCraft)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnExitCraftIsDestroying");

	if (!Func) {
		return;
	}

	Call(Func, ExitCraft);
}

void AFortAthenaMutator_Heist::SendPlayerAcquiredMessage(AFortPlayerStateAthena* CarryingPlayerState)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("SendPlayerAcquiredMessage");

	if (!Func) {
		return;
	}

	Call(Func, CarryingPlayerState);
}

void AFortAthenaMutator_Heist::OnMutatorGameplayEvent(int32 EventId, int32 EventParam1, int32 EventParam2)
{
	Log(std::format("AFortAthenaMutator_Heist::OnMutatorGameplayEvent: EventId={} EventParam1={} EventParam2={}", EventId, EventParam1, EventParam2));
}

void AFortAthenaMutator_Heist::execOnMutatorGameplayEvent(AFortAthenaMutator_Heist* Context, FFrame& Stack) {
	struct FortAthenaMutator_Heist_OnMutatorGameplayEvent
	{
	public:
		int32 EventId;
		int32 EventParam1;
		int32 EventParam2;
	};
	FortAthenaMutator_Heist_OnMutatorGameplayEvent* Params = (FortAthenaMutator_Heist_OnMutatorGameplayEvent*)Stack.Locals;

	execOnMutatorGameplayEventOG(Context, Stack);
	Context->OnMutatorGameplayEvent(Params->EventId, Params->EventParam1, Params->EventParam2);
}
