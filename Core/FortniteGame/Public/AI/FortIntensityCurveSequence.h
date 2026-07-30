#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

class UFortIntensityCurveSequence : public UDataAsset {
public:
	DefineUnrealClass(UFortIntensityCurveSequence);

	DefineUProperty(TArray<FDataTableRowHandle>, IntensityCurves);
};
