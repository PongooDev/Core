#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"

#include "FortniteGame/Public/FortEnums.h"
#include "FortniteGame/Public/Athena/ExitCraftInfo.h"


class AFortAthenaExitCraft;
class AFortAthenaExitCraftSpawner;

class UFortAthenaExitCraftInfo : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortAthenaExitCraftInfo);

	DefineUProperty(TSubclassOf<AFortAthenaExitCraft>, ExitCaftClass);
	DefineUProperty(TSubclassOf<AFortAthenaExitCraftSpawner>, ExitCraftSpawnerClass);
	DefineUProperty(FExitCraftInfo, ExitCraftInfo);
	DefineUProperty(FGameplayTag, SpecialActorSpawnerTag);
	DefineUProperty(FSlateBrush, SpawnerMinimapIconBrush);
	DefineUProperty(FVector2D, SpawnerMinimapIconScale);
	DefineUProperty(FSlateBrush, SpawnerCompassIconBrush);
	DefineUProperty(FVector2D, SpawnerCompassIconScale);
	DefineUProperty(FGameplayTag, SpecialActorCraftTag);
	DefineUProperty(FSlateBrush, CraftMinimapIconBrush);
	DefineUProperty(FVector2D, CraftMinimapIconScale);
	DefineUProperty(FSlateBrush, CraftCompassIconBrush);
	DefineUProperty(FVector2D, CraftCompassIconScale);
};
