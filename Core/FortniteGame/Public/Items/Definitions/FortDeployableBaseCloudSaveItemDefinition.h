#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortCloudSaveItemDefinition.h"

class UFortDeployableBaseCloudSaveItemDefinition : public UFortCloudSaveItemDefinition {
public:
	DefineUnrealClass(UFortDeployableBaseCloudSaveItemDefinition);

	DefineUProperty(FGuid, SaveFilenameGUID);
};
