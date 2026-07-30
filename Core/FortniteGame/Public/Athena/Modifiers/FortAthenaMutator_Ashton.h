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
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UFortGadgetItemDefinition;
class UFortWorldItemDefinition;

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
class UFortMissionInfo;
class UFortMovementComp_AIChar;
class UFortServerBotManagerAthena;
class UFortTeamSettingsComponent;
class UGameplayEffect;
class UMaterialInstance;
class USkeletalMesh;
class USoundCue;
class UStaticMesh;
class UTexture2D;

class EAshtonStoneStateType {
public:
	DefineUnrealEnum(EAshtonStoneStateType);

	DefineEnumProperty(NotSpawned);
	DefineEnumProperty(Spawned);
	DefineEnumProperty(Captured);
	DefineEnumProperty(MAX);
};

class EAshtonStoneType {
public:
	DefineUnrealEnum(EAshtonStoneType);

	DefineEnumProperty(Purple);
	DefineEnumProperty(Blue);
	DefineEnumProperty(Red);
	DefineEnumProperty(Orange);
	DefineEnumProperty(Green);
	DefineEnumProperty(Yellow);
	DefineEnumProperty(MAX);
};

struct FAshtonStoneState {
public:
	DefineUnrealStruct(FAshtonStoneState);

	DefineStructProperty(EAshtonStoneType, StoneType);
	DefineStructProperty(EAshtonStoneStateType, StoneState);
	DefineStructProperty(FGameplayTag, GameplayTag);
	DefineStructProperty(float, SpawnTime);
	DefineStructProperty(bool, bHasEverSpawned);
	DefineStructProperty(int32, SpawnDataIdx);
public:
	uint8 Padding[0x18];
};

class AFortAthenaMutator_Ashton : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Ashton);

	DefineUProperty(FScalableFloat, GameOverDelay);
	DefineUProperty(TArray<FAshtonStoneState>, StoneList);
	DefineUProperty(TArray<FAshtonStoneState>, CachedStoneList);
	DefineUProperty(FVector, VillainLeaderLocationForSnapSound);
	DefineUProperty(FVector, VillainLeaderLocationForDeathSound);
	DefineUProperty(FVector, VillainLeaderLocationForSound);
	DefineUProperty(FGameplayTag, JimInventoryTag);
	DefineUProperty(FGameplayTag, JimPickupTag);
	DefineUProperty(FGameplayTag, VillainTeamTag);
	DefineUProperty(FGameplayTag, HeroTeamTag);
	DefineUProperty(FGameplayTagContainer, ForceEquipTags);
	DefineUProperty(FSlateBrush, MapEnemyBrush);
	DefineUProperty(FVector2D, MapEnemyScale);
	DefineUProperty(AFortPlayerController*, AutoEquipController);
	DefineUProperty(UFortGadgetItemDefinition*, VillainLeaderItemDef);
	DefineUProperty(TArray<UFortWorldItemDefinition*>, VillainItemDefs);
	DefineUProperty(AFortPlayerControllerAthena*, VillainLeaderPC);
	DefineUProperty(FScalableFloat, NumVillainRespawns);
	DefineUProperty(int32, NumVillainRespawnsRemaining);
	DefineUProperty(FScalableFloat, PromoteNextVillainLeaderDelay);
	DefineUProperty(FScalableFloat, VillainsDisplayStoneIcons);
	DefineUProperty(int32, CachedNumCapturedStones);
	DefineUProperty(FAthenaGameMessageData, GameMsg_IntroVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_IntroHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_HeroFollowMap);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneCapturedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FirstStoneCapturedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneCapturedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StoneCapturedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_OneStoneLeftVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_OneStoneLeftHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FinalStoneComingVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_FinalStoneComingHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EliminatedThanosVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_EliminatedThanosHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnThanosVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_RespawnThanosHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StonesCompleteVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_StonesCompleteHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_VillainArmyDepletedVillain);
	DefineUProperty(FAthenaGameMessageData, GameMsg_VillainArmyDepletedHero);
	DefineUProperty(FAthenaGameMessageData, GameMsg_NoMoreRespawns);
};
