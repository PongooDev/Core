#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "FortniteGame/Public/Items/ItemAndCount.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "FortniteGame/Public/Athena/Modifiers/MutatorObjectData.h"

class AActor;
class AAthenaBarrierFlag;
class AAthenaBigBaseWall;
class AAthenaFillFloor;
class AAthenaGoosePickup;
class AAthenaSkyCap;
class ABuildingActor;
class ABuildingGameplayActorSpawnMachine;
class AFortAthenaSupplyDrop;
class AFortAthenaVehicle;
class AFortGameModeAthena;
class AFortGameStateAthena;
class AFortPlayerController;
class AFortPlayerControllerAthena;
class AFortPlayerPawnAthena;
class AFortPlayerStateAthena;
class UAnimInstance;
class UCustomCharacterPart;
class UDataAsset;
class UEnvQuery;
class UFortAIEncounterInfo;
class UFortAISpawnGroup;
class UFortAthenaExitCraftInfo;
class UFortAthenaMutator_SpawningPolicyData;
class UFortDifficultyEncounterSettings;
class UFortGadgetItemDefinition;
class UFortMissionInfo;
class UFortMovementComp_AIChar;
class UFortServerBotManagerAthena;
class UFortTeamSettingsComponent;
class UFortWorldItemDefinition;
class UGameplayEffect;
class UMaterialInstance;
class USkeletalMesh;
class USoundCue;
class UStaticMesh;
class UTexture2D;

struct FFortAthenaMutator_SurvivalObjectiveData {
public:
	DefineUnrealStruct(FFortAthenaMutator_SurvivalObjectiveData);

	DefineStructProperty(TSubclassOf<ABuildingActor>, BuildingActorObjectiveClass);
	DefineStructProperty(FScalableFloat, SpawnDistanceFromGround);
	DefineStructProperty(int32, ActivationSafezoneIndex);
	DefineStructProperty(bool, bClearAreaOnSpawn);
	DefineStructProperty(float, ClearAreaRadiusOverride);
	DefineStructProperty(float, ClearAreaHalfHeightOverride);
	DefineStructProperty(FGameplayTagQuery, POIFilterQuery);
	DefineStructProperty(int32, RandomizedPOICount);
	DefineStructProperty(TArray<int32>, ExtraSafezoneIndexes);
	DefineStructProperty(ABuildingActor*, SpawnedBuildingActorObjective);
	DefineStructProperty(FGameplayTag, SpecialActorTag);
	DefineStructProperty(FSlateBrush, SpecialActorMinimapIconBrush);
	DefineStructProperty(FVector2D, SpecialActorMinimapIconScale);
	DefineStructProperty(FSlateBrush, SpecialActorCompassIconBrush);
	DefineStructProperty(FVector2D, SpecialActorCompassIconScale);
	DefineStructProperty(FName, SpecialActorID);
	DefineBitfieldStructProperty(bEndMatchOnDestroy);
	DefineBitfieldStructProperty(bSpawnOnPOI);
	DefineBitfieldStructProperty(bIsSpecialActor);
public:
	uint8 Padding[0x1D8];
};

class AFortAthenaMutator_DeimosSurvival : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_DeimosSurvival);

	DefineUProperty(TSoftObjectPtr<UEnvQuery>, AdjustSafezoneCentersQuery);
	DefineUProperty(TArray<FFortAthenaMutator_SurvivalObjectiveData>, ObjectivesData);
	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
	DefineUProperty(FAthenaGameMessageData, GameMsg_CompleteObjective);
	DefineUProperty(FAthenaGameMessageData, GameMsg_DownTime);
};
