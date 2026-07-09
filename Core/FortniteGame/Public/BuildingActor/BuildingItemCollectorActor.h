#pragma once
#include "pch.h"

#include "BuildingGameplayActor.h"

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
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/FortItem/FortItemEntry.h"

class AFortPlayerController;
class UFortWorldItemDefinition;
class UTexture;

struct FColletorUnitInfo {
public:
	DefineUnrealStruct(FColletorUnitInfo);

	DefineStructProperty(UFortWorldItemDefinition*, InputItem);
	DefineStructProperty(FScalableFloat, InputCount);
	DefineStructProperty(UTexture*, OverrideInputItemTexture);
	DefineBitfieldStructProperty(bUseDefinedOutputItem);
	DefineStructProperty(UFortWorldItemDefinition*, OutputItem);
	DefineStructProperty(TArray<FFortItemEntry>, OutputItemEntry);
	DefineStructProperty(FName, OverrideOutputItemLootTierGroupName);
	DefineStructProperty(UTexture*, OverrideOutputItemTexture);
};

class ABuildingItemCollectorActor : public ABuildingGameplayActor {
public:
	DefineUnrealClass(ABuildingItemCollectorActor);

	DefineUProperty(uint8, ItemCollectorBehavior);
	DefineUProperty(bool, bRequireAllForInteraction);
	DefineUProperty(bool, bRequireAllForTurnIn);
	DefineUProperty(bool, bHasToHaveSomething);
	DefineUProperty(bool, bIncrementCaptureCountWhenGoalIsReached);
	DefineUProperty(bool, bCurrentInteractionSuccess);
	DefineUProperty(uint8, ItemCollectorState);
	DefineUProperty(uint8, TrackingType);
	DefineUProperty(AFortPlayerController*, ControllingPlayer);
	DefineUProperty(FName, DefaultItemLootTierGroupName);
	DefineUProperty(TArray<FColletorUnitInfo>, ItemCollections);
	DefineUProperty(UFortWorldItemDefinition*, ActiveInputItem);
	DefineUProperty(UFortWorldItemDefinition*, ClientPausedActiveInputItem);
	DefineUProperty(FVector, LootSpawnLocation);
	DefineUProperty(FRotator, LootTossDirection);
	DefineUProperty(int32, StartingGoalLevel);
	DefineUProperty(bool, bTossOnGround);
public:
	bool GrantOutput();

	bool Setup();

	static inline void (*BeginPlayOG)(ABuildingItemCollectorActor* This);
	static void BeginPlay(ABuildingItemCollectorActor* This);

	static void Hook();
};