#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayEffectTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"

#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Building/BuildingActor.h"

class ABuildingActor;
class ABuildingSMActor;
class AFortPlayerController;
class AFortPlayerStateZone;
class UFortWorldItemDefinition;
class UFortBuildingActorSet;
class USceneComponent;

class ABGAConsumableSpawner : public ABuildingActor {
public:
	DefineUnrealClass(ABGAConsumableSpawner);

	DefineUProperty(USceneComponent*, DummyRoot);
	DefineUProperty(FName, SpawnLootTierGroup);
	DefineUProperty(ABuildingActor*, AssociatedBuildingActor);
	DefineUProperty(TArray<FFortItemEntry>, ConsumablesToSpawn);
	DefineUProperty(bool, bAlignSpawnedActorsToSurface);
public:
	bool AttemptSpawn();
};