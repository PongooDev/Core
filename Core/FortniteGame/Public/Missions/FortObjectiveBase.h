#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/FortTeamInfo.h"
#include "FortniteGame/Public/Missions/FortMissionState.h"

class UFortBadgeItemDefinition;
class UFortMissionTimerComponent;
class UFortWorldItemDefinition;

class EFortMissionType {
public:
	DefineUnrealEnum(EFortMissionType);

	DefineEnumProperty(Primary);
	DefineEnumProperty(Secondary);
	DefineEnumProperty(Max_None);
};

class EFortObjectiveRequirement {
public:
	DefineUnrealEnum(EFortObjectiveRequirement);

	DefineEnumProperty(Optional);
	DefineEnumProperty(Required);
	DefineEnumProperty(RequiredButCanFail);
};

class EFortMissionAudibility {
public:
	DefineUnrealEnum(EFortMissionAudibility);

	DefineEnumProperty(UseVisibility);
	DefineEnumProperty(Audible);
	DefineEnumProperty(Inaudible);
};

class EFortObjectiveStatus {
public:
	DefineUnrealEnum(EFortObjectiveStatus);

	DefineEnumProperty(Created);
	DefineEnumProperty(InProgress);
	DefineEnumProperty(Succeeded);
	DefineEnumProperty(Failed);
	DefineEnumProperty(NeutralCompletion);
	DefineEnumProperty(Max_None);
};

class EFortMissionVisibilityOverride {
public:
	DefineUnrealEnum(EFortMissionVisibilityOverride);

	DefineEnumProperty(Visible);
	DefineEnumProperty(Invisible);
	DefineEnumProperty(Max_None);
};

class AFortObjectiveBase : public AFortMissionState {
public:
	DefineUnrealClass(AFortObjectiveBase);

	DefineUProperty(UFortBadgeItemDefinition*, ObjectiveRewardBadge);
	DefineUProperty(TArray<UFortWorldItemDefinition*>, ItemsToGiveOnObjectiveStart);
	DefineUProperty(bool, bStartPlayingOnMissionStart);
	DefineUProperty(bool, bAcceptsMissionEventsWhenFinished);
	DefineUProperty(FGameplayTagContainer, ObjectiveHandle);
	DefineUProperty(FString, ObjectiveAnalyticsName);
	DefineUProperty(EFortObjectiveRequirement, MissionRequirement);
	DefineUProperty(bool, bIsObjectiveVisible);
	DefineUProperty(EFortMissionVisibilityOverride, VisibilityOverride);
	DefineUProperty(bool, bIsProgressBarHidden);
	DefineUProperty(EFortMissionAudibility, ObjectiveAudiblity);
	DefineUProperty(bool, bRelevantToSpecificTeam);
	DefineUProperty(EFortTeam, RelevantTeam);
	DefineUProperty(EFortObjectiveStatus, ObjectiveStatus);
	DefineUProperty(UFortMissionTimerComponent*, TimerComponent);
};
