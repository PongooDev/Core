#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"


class AFortAthenaExitCraft;
class AFortAthenaExitCraftSpawner;

struct FExitCraftInfo {
public:
	DefineUnrealStruct(FExitCraftInfo);

	DefineStructProperty(FScalableFloat, ExitCraftSpawnerZOffset);
	DefineStructProperty(FScalableFloat, ExitCraftSpawnDelay);
	DefineStructProperty(FScalableFloat, ExitCraftZOffset);
	DefineStructProperty(FScalableFloat, ExitCraftTargetZOffset);
	DefineStructProperty(FScalableFloat, ExitCraftTimeToHoverLocation);
	DefineStructProperty(FScalableFloat, ExitCraftTimeToHoverRotation);
	DefineStructProperty(FScalableFloat, ExitZOffset);
	DefineStructProperty(FScalableFloat, ExitTime);
	DefineStructProperty(FScalableFloat, InteractionTime);
public:
	uint8 Padding[0x1E8];
};
