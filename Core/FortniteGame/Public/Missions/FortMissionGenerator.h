#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "FortniteGame/Public/FortGameData.h"

class UFortMissionInfo;

class UFortTips;
class UTexture2D;

struct FFortAthenaLTMConfig {
public:
	DefineUnrealStruct(FFortAthenaLTMConfig);

	DefineStructProperty(TSoftObjectPtr<UTexture2D>, SplashImage);
	DefineStructProperty(FText, FrontEndDescription);
	DefineStructProperty(FText, DisabledMessage);
public:
	uint8 Padding[0x58];
};

struct FZoneLoadingScreenConfig {
public:
	DefineUnrealStruct(FZoneLoadingScreenConfig);

	DefineStructProperty(TSoftObjectPtr<UTexture2D>, BackgroundImage);
	DefineStructProperty(FText, TitleDescription);
	DefineStructProperty(FText, Title);
	DefineStructProperty(TArray<UFortTips*>, LoadingTips);
public:
	uint8 Padding[0x110];
};

struct FFortMultiSizeBrush {
public:
	DefineUnrealStruct(FFortMultiSizeBrush);

	DefineStructProperty(FSlateBrush, Brush_XXS);
	DefineStructProperty(FSlateBrush, Brush_XS);
	DefineStructProperty(FSlateBrush, Brush_S);
	DefineStructProperty(FSlateBrush, Brush_M);
	DefineStructProperty(FSlateBrush, Brush_L);
	DefineStructProperty(FSlateBrush, Brush_XL);
public:
	uint8 Padding[0x330];
};

struct FFortPossibleMission {
public:
	DefineUnrealStruct(FFortPossibleMission);

	DefineStructProperty(TSoftObjectPtr<UFortMissionInfo>, MissionInfo);
	DefineStructProperty(float, Weight);
	DefineStructProperty(int32, MinAlwaysGenerated);
	DefineStructProperty(bool, bIsPrototype);
public:
	uint8 Padding[0x31];
};

class UFortMissionGenerator : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortMissionGenerator);

	DefineUProperty(FText, MissionName);
	DefineUProperty(FText, MissionDescription);
	DefineUProperty(FSlateBrush, MissionIcon);
	DefineUProperty(FFortMultiSizeBrush, MissionIcons);
	DefineUProperty(FZoneLoadingScreenConfig, LoadingScreenConfig);
	DefineUProperty(FFortAthenaLTMConfig, AthenaLTMConfig);
	DefineUProperty(TSoftObjectPtr<UFortMissionInfo>, PrimaryMissionInfo);
	DefineUProperty(bool, bUseNewMissionGeneration);
	DefineUProperty(FGameplayTagQuery, OptionalRequirementsForAllOtherMissions);
	DefineUProperty(TArray<FFortPossibleMission>, SecondaryMissionList);
	DefineUProperty(TArray<FFortPossibleMission>, TertiaryMissionList);
	DefineUProperty(TArray<FFortPossibleMission>, SurvivorMissionList);
	DefineUProperty(bool, bUseOverridePlayerSpawnPadPlacementData);
	DefineUProperty(FFortPlayerSpawnPadPlacementData, OverridePlayerSpawnPadPlacementData);
};
