#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"

class UFortMissionInfo;

class UFortMissionGenerator : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortMissionGenerator);

	DefineUProperty(FText, MissionName);
	DefineUProperty(FText, MissionDescription);
	DefineUProperty(FSlateBrush, MissionIcon);
	DefineUProperty(TSoftObjectPtr<UFortMissionInfo>, PrimaryMissionInfo);
	DefineUProperty(bool, bUseNewMissionGeneration);
	DefineUProperty(FGameplayTagQuery, OptionalRequirementsForAllOtherMissions);
	DefineUProperty(bool, bUseOverridePlayerSpawnPadPlacementData);
};
