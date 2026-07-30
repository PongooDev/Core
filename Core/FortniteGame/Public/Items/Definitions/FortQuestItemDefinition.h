#pragma once
#include "pch.h"

#include "FortniteGame/Public/McpItemDefinitionBase.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/BlueprintFunctionLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"
#include "Engine/Source/Runtime/Core/Public/Misc/OutputDevice.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/ScriptInterface.h"
#include "Engine/Source/Runtime/Net/Core/Classes/Net/Serialization/FastArraySerializer.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/World.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/Quests/FortMcpQuestObjectiveInfo.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortWeaponMeleeItemDefinition;
class USkeletalMesh;
class UAnimInstance;
class USoundBase;


class UFortQuestItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortQuestItemDefinition);

	DefineUProperty(int32, ExpirationDuration);
	DefineUProperty(TArray<FFortMcpQuestObjectiveInfo>, Objectives);
	DefineUProperty(FGameplayTagContainer, Prerequisites);
	DefineUProperty(TSoftObjectPtr<UFortQuestItemDefinition>, PrerequisiteQuest);
	DefineUProperty(FDataTableRowHandle, PrerequisiteObjective);
	DefineUProperty(float, Weight);
	DefineUProperty(bool, IsStreamingRequired);
	DefineUProperty(FDataTableRowHandle, Category);
	DefineUProperty(bool, bExpandsStormShield);
	DefineUProperty(bool, bHidden);
	DefineUProperty(bool, bAutoLaunch);
	DefineUProperty(bool, bDeprecated);
	DefineUProperty(bool, bDisableBackendConditionEvaluation);
	DefineUProperty(bool, bAllowTileMatching);
	DefineUProperty(bool, bAllowMissionAlertMatchesBypassingTileRequirements);
};
