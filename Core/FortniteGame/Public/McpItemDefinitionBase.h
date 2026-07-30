#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/ScalableFloat.h"

#include "FortniteGame/Public/Items/FortItem.h"

class UMcpItemDefinitionBase : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UMcpItemDefinitionBase);
};