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
#include "FortniteGame/Public/FortEnums.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "FortniteGame/Public/Items/ItemAndCount.h"
#include "FortniteGame/Public/Athena/AthenaGameMessageData.h"
#include "FortniteGame/Public/Athena/Modifiers/MutatorObjectData.h"

// The comma would split DefineUProperty into three macro arguments.
using FTeamSettingsDataMap = TMap<uint8, FTeamSettingsData>;

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

struct FTeamSettingsData {
public:
	DefineUnrealStruct(FTeamSettingsData);

	DefineStructProperty(UFortTeamSettingsComponent*, TeamSettings);
public:
	uint8 Padding[0x18];
};

class AFortAthenaMutator_TeamSettings : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TeamSettings);

	DefineUProperty(TSubclassOf<UFortTeamSettingsComponent>, TeamSettingsClass);
	DefineUProperty(TArray<FGravityMovementData>, GravityData);
	DefineUProperty(FTeamSettingsDataMap, TeamSettingsDataMap);
	DefineUProperty(UFortTeamSettingsComponent*, DefaultTeamSettings);
	DefineUProperty(UFortTeamSettingsComponent*, CurrentTeamSettings);
};
