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

class EAthenaTimeOfDayOverride {
public:
	DefineUnrealEnum(EAthenaTimeOfDayOverride);

	DefineEnumProperty(NoOverride);
	DefineEnumProperty(ForceDay);
	DefineEnumProperty(ForceNight);
	DefineEnumProperty(Custom);
	DefineEnumProperty(Hour0);
	DefineEnumProperty(Hour1);
	DefineEnumProperty(Hour2);
	DefineEnumProperty(Hour3);
	DefineEnumProperty(Hour4);
	DefineEnumProperty(Hour5);
	DefineEnumProperty(Hour6);
	DefineEnumProperty(Hour7);
	DefineEnumProperty(Hour8);
	DefineEnumProperty(Hour9);
	DefineEnumProperty(Hour10);
	DefineEnumProperty(Hour11);
	DefineEnumProperty(Hour12);
	DefineEnumProperty(Hour13);
	DefineEnumProperty(Hour14);
	DefineEnumProperty(Hour15);
	DefineEnumProperty(Hour16);
	DefineEnumProperty(Hour17);
	DefineEnumProperty(Hour18);
	DefineEnumProperty(Hour19);
	DefineEnumProperty(Hour20);
	DefineEnumProperty(Hour21);
	DefineEnumProperty(Hour22);
	DefineEnumProperty(Hour23);
	DefineEnumProperty(Random);
	DefineEnumProperty(EAthenaTimeOfDayOverride_MAX);
};

struct FTimeOfDayPhase {
public:
	DefineUnrealStruct(FTimeOfDayPhase);

	DefineStructProperty(float, Time);
	DefineStructProperty(FText, DisplayName);
public:
	uint8 Padding[0x20];
};

struct FTimeOfDaySpeed {
public:
	DefineUnrealStruct(FTimeOfDaySpeed);

	DefineStructProperty(float, Speed);
	DefineStructProperty(FText, DisplayName);
public:
	uint8 Padding[0x20];
};

class AFortAthenaMutator_TODOverride : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_TODOverride);

	DefineUProperty(EAthenaTimeOfDayOverride, TimeOfDayOverride);
	DefineUProperty(float, TimeOverride);
	DefineUProperty(float, SpeedOverride);
	DefineUProperty(TArray<FTimeOfDayPhase>, TimeOfDayPhases);
	DefineUProperty(TArray<FTimeOfDaySpeed>, TimeOfDaySpeeds);
};
