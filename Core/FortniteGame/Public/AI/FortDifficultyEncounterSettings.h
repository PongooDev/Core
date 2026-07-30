#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "FortniteGame/Public/AI/EncounterEnvironmentQueryInfo.h"
#include "FortniteGame/Public/AI/FortEncounterSettings.h"

class EFortEncounterPacingMode {
public:
	DefineUnrealEnum(EFortEncounterPacingMode);

	DefineEnumProperty(SpawnPointsPercentageCurve);
	DefineEnumProperty(IntensityCurve);
	DefineEnumProperty(Burst);
	DefineEnumProperty(Fixed);
};

class EFortEncounterSpawnLimitType {
public:
	DefineUnrealEnum(EFortEncounterSpawnLimitType);

	DefineEnumProperty(NoLimit);
	DefineEnumProperty(NumPawnsLimit);
	DefineEnumProperty(SpawnPointLimit);
	DefineEnumProperty(UserDefined);
	DefineEnumProperty(MAX);
};

class UFortDifficultyEncounterSettings : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortDifficultyEncounterSettings);

	DefineUProperty(FEncounterEnvironmentQueryInfo, OverrideEncounterEnvironmentQueryInfo);
	DefineUProperty(FFortEncounterSettings, EncounterSettings);
	DefineUProperty(EFortEncounterSpawnLimitType, SpawnLimitMode);
	DefineUProperty(EFortEncounterPacingMode, PacingMode);
};
