#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class AFortMissionState : public AActor {
public:
	DefineUnrealClass(AFortMissionState);

	DefineUProperty(bool, bAlreadySetTimerOnce);
	DefineUProperty(bool, bAlreadyCalledOnAllPlayersLoadedIn);
	DefineUProperty(float, TimerForOnAllPlayersLoadedIn);
};
