#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/Missions/FortMissionState.h"

class UFortBadgeItemDefinition;
class UFortMissionTimerComponent;
class UFortWorldItemDefinition;

class AFortObjectiveBase : public AFortMissionState {
public:
	DefineUnrealClass(AFortObjectiveBase);

	DefineUProperty(UFortBadgeItemDefinition*, ObjectiveRewardBadge);
	DefineUProperty(TArray<UFortWorldItemDefinition*>, ItemsToGiveOnObjectiveStart);
	DefineUProperty(bool, bStartPlayingOnMissionStart);
	DefineUProperty(bool, bAcceptsMissionEventsWhenFinished);
	DefineUProperty(FGameplayTagContainer, ObjectiveHandle);
	DefineUProperty(FString, ObjectiveAnalyticsName);
	DefineUProperty(bool, bIsObjectiveVisible);
	DefineUProperty(bool, bIsProgressBarHidden);
	DefineUProperty(bool, bRelevantToSpecificTeam);
	DefineUProperty(UFortMissionTimerComponent*, TimerComponent);
};
