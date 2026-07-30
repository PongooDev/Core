#include "pch.h"
#include "FortniteGame/Public/Missions/FortMissionManager.h"

#include "FortniteGame/Public/Missions/FortMission.h"

void AFortMissionManager::OnRep_Missions()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Missions");

	if (!Func) {
		Log("AFortMissionManager::OnRep_Missions: Failed to find function!");
		return;
	}

	return const_cast<AFortMissionManager*>(this)->Call<void>(Func);
}
