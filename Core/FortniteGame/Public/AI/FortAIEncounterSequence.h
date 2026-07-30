#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/AI/EncounterEnvironmentQueryInfo.h"
#include "FortniteGame/Public/AI/FortEncounterSettings.h"

class AActor;
class AFortAIDirector;
class AFortMission;
class UFortAIAssignmentSettings;
class UFortAIEncounterInfo;

class UFortAIEncounterSequence : public UObject {
public:
	DefineUnrealClass(UFortAIEncounterSequence);

	DefineUProperty(int32, CurrentEncounterIndexInSequence);
	DefineUProperty(UFortAIEncounterInfo*, CurrentEncounter);
	DefineUProperty(AFortAIDirector*, AssociatedAIDirector);
	DefineUProperty(AFortMission*, AssociatedMission);
	DefineUProperty(TArray<AActor*>, TargetActors);
	DefineUProperty(UFortAIAssignmentSettings*, AssignmentSettings);
	DefineUProperty(FEncounterEnvironmentQueryInfo, OverrideEnvironmentQueryInfo);
	DefineUProperty(AActor*, OptionalQueryActor);
	DefineUProperty(FGameplayTagContainer, InjectedTags);
	DefineUProperty(FFortEncounterSettings, EncounterSettings);
};
