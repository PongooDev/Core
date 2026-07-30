#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataTable.h"

class EFortIntensityCurveSequenceType {
public:
	DefineUnrealEnum(EFortIntensityCurveSequenceType);

	DefineEnumProperty(Sequence);
	DefineEnumProperty(Loop);
	DefineEnumProperty(Random);
	DefineEnumProperty(Max_None);
};

class UFortIntensityCurveSequence : public UDataAsset {
public:
	DefineUnrealClass(UFortIntensityCurveSequence);

	DefineUProperty(TArray<FDataTableRowHandle>, IntensityCurves);
	DefineUProperty(EFortIntensityCurveSequenceType, SequenceType);
};
