#include "pch.h"
#include "FortniteGame/Public/Kismet/FortMissionLibrary.h"

void UFortMissionLibrary::LoadMission(UObject* WorldContextObject, UFortMissionInfo* MissionInfo)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/FortniteGame.FortMissionLibrary.LoadMission");

	if (!Func)
	{
		Log("UFortMissionLibrary::LoadMission: Failed to find function!");
		return;
	}

	return GetDefaultObj()->Call<void>(Func, WorldContextObject, MissionInfo);
}