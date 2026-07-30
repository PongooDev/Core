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

struct FObjectiveSpecialActorContainer {
public:
	DefineUnrealStruct(FObjectiveSpecialActorContainer);

	DefineStructProperty(AActor*, TheSpawnedObjective);
public:
	uint8 Padding[0x10];
};

class AFortAthenaMutator_SpawningPolicyBase : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_SpawningPolicyBase);

	DefineUProperty(TArray<FObjectiveSpecialActorContainer>, SpecialActorIDList);
	DefineUProperty(int32, CachedSpecialActorIdx);
	DefineUProperty(float, SpawnFailureLockoutTime);
	DefineUProperty(EAthenaGamePhase, GamePhaseToStartSpawning);
	DefineUProperty(UFortAthenaMutator_SpawningPolicyData*, ItemDataRemovalQueryPending);
	DefineUProperty(FScalableFloat, MinRespawnTime);
	DefineUProperty(FScalableFloat, MaxRespawnTime);
	DefineBitfieldUProperty(bShouldCenterGroundCheckAtFoundLocation);
	DefineBitfieldUProperty(bShouldMaintainItemCount);
	DefineBitfieldUProperty(bAllowedDespawnToMaintainItemCount);
};
