#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortExpeditionItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortExpeditionItemDefinition);

	DefineUProperty(FString, CriteriaRequirementsLootTier);
	DefineUProperty(float, ExpeditionDuration_Minutes);
	DefineUProperty(float, ExpeditionExpirationDuration_Minutes);
	DefineUProperty(int32, BaseTargetPowerRating);
	DefineUProperty(int32, TierFactor);
	DefineUProperty(int32, MaxTargetPowerClamp);
	DefineUProperty(FDataTableRowHandle, ExpeditionRules);
};
