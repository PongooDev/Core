#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/AI/FortCurveSequenceInstanceInfo.h"

class UFortSpawnPointsPercentageCurveSequence;

struct FFortSpawnPointsPercentageCurveSequenceInstanceInfo : public FFortCurveSequenceInstanceInfo {
public:
	DefineUnrealStruct(FFortSpawnPointsPercentageCurveSequenceInstanceInfo);

	DefineStructProperty(UFortSpawnPointsPercentageCurveSequence*, SpawnPointsPercentageCurveSequence);
public:
	uint8 Padding[0x10];
};
