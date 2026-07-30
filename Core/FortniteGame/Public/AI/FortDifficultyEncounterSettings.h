#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/DataAsset.h"
#include "FortniteGame/Public/AI/EncounterEnvironmentQueryInfo.h"
#include "FortniteGame/Public/AI/FortEncounterSettings.h"

class UFortDifficultyEncounterSettings : public UPrimaryDataAsset {
public:
	DefineUnrealClass(UFortDifficultyEncounterSettings);

	DefineUProperty(FEncounterEnvironmentQueryInfo, OverrideEncounterEnvironmentQueryInfo);
	DefineUProperty(FFortEncounterSettings, EncounterSettings);
};
