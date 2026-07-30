#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"

class UFortAlterationItemDefinition;
class UFortWorldItemDefinition;
class USkeletalMesh;
class USoundBase;
class UStaticMesh;

class AFortPickupEffect : public AActor {
public:
	DefineUnrealClass(AFortPickupEffect);

	DefineUProperty(UStaticMesh*, StaticMesh);
	DefineUProperty(USkeletalMesh*, SkeletalMesh);
	DefineUProperty(TArray<UFortAlterationItemDefinition*>, ActiveAlterations);
	DefineUProperty(UFortWorldItemDefinition*, ItemDefinition);
	DefineUProperty(bool, bDoNotShowSpawnParticles);
	DefineUProperty(bool, bDoNotTickSkeletalMeshComponents);
	DefineUProperty(FVector, PickupColor);
	DefineUProperty(FLinearColor, BackpackWorldPosition);
	DefineUProperty(bool, bRandomRotation);
	DefineUProperty(TSoftObjectPtr<USoundBase>, PickupByNearbyPawnSound);
};
