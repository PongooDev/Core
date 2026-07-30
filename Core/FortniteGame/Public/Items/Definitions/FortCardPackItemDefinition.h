#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortPackPersonality;
class UTexture;
class UTexture2D;

class UFortCardPackItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortCardPackItemDefinition);

	DefineUProperty(bool, bIsLlama);
	DefineUProperty(bool, bIsChoicePack);
	DefineUProperty(bool, bAutoOpenAsReward);
	DefineUProperty(int32, LootTier);
	DefineUProperty(FString, LootTierGroup);
	DefineUProperty(int32, DisplayRarityLevel);
	DefineUProperty(TSoftObjectPtr<UTexture>, PackImage);
	DefineUProperty(TSoftObjectPtr<UTexture2D>, XRayTexture);
	DefineUProperty(FLinearColor, PackColor);
	DefineUProperty(TSoftObjectPtr<UFortPackPersonality>, PackPersonality);
};
