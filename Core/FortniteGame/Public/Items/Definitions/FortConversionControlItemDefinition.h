#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortConversionControlItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortConversionControlItemDefinition);

	DefineUProperty(FString, LootTierGroup);
	DefineUProperty(FDataTableRowHandle, ConversionPointsToTierCosts);
	DefineUProperty(bool, bConsumedOnConversion);
	DefineUProperty(FText, RewardName);
};
