#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/AI/FortCurveSequenceInstanceInfo.h"

class UFortIntensityCurveSequence;

struct FFortIntensityCurveSequenceInstanceInfo : public FFortCurveSequenceInstanceInfo {
public:
	DefineUnrealStruct(FFortIntensityCurveSequenceInstanceInfo);

	DefineStructProperty(UFortIntensityCurveSequence*, IntensityCurveSequence);
public:
	uint8 Padding[0x10];
};
