#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Math/Color.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

class UFortBadgeItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortBadgeItemDefinition);

	DefineUProperty(FText, DisplayText);
	DefineUProperty(FText, FailedDisplayText);
	DefineUProperty(FLinearColor, BadgeColor);
	DefineUProperty(FDataTableRowHandle, BadgeScoringValuesHandle);
};
