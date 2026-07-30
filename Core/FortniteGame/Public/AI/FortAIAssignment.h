#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "FortniteGame/Public/FortTeamInfo.h"

class AFortAIController;
class UFortAIAssignmentSettings;
class UFortAIGoalProvider;

class AActor;

class EAssignmentType {
public:
	DefineUnrealEnum(EAssignmentType);

	DefineEnumProperty(Invalid);
	DefineEnumProperty(Encounter);
	DefineEnumProperty(World);
	DefineEnumProperty(Enemy);
	DefineEnumProperty(NumAssignmentTypes);
};

struct FFortAIAssignmentIdentifier {
public:
	DefineUnrealStruct(FFortAIAssignmentIdentifier);

	DefineStructProperty(EAssignmentType, AssignmentType);
	DefineStructProperty(FGameplayTagContainer, AssignmentGameplayTags);
	DefineStructProperty(EFortTeam, AssignmentTeam);
public:
	uint8 Padding[0x29];
};

struct FFortAIGoalInfo {
public:
	DefineUnrealStruct(FFortAIGoalInfo);

	DefineStructProperty(TWeakObjectPtr<AActor>, Actor);
	DefineStructProperty(FVector, Location);
	DefineStructProperty(bool, bActorAlwaysPerceived);
public:
	uint8 Padding[0x15];
};

struct FFortAIGoal : public FFortAIGoalInfo {
public:
	DefineUnrealStruct(FFortAIGoal);

	DefineStructProperty(TArray<AFortAIController*>, ControllersAssignedToGoal);
public:
	uint8 Padding[0x28];
};

class UFortAIAssignment : public UObject {
public:
	DefineUnrealClass(UFortAIAssignment);

	DefineUProperty(FFortAIAssignmentIdentifier, AssignmentIdentifier);
	DefineUProperty(UFortAIAssignmentSettings*, AssignmentSettings);
	DefineUProperty(UFortAIGoalProvider*, GoalProvider);
	DefineUProperty(TArray<AFortAIController*>, ControllersOnAssignment);
	DefineUProperty(TArray<FFortAIGoal>, Goals);
};
