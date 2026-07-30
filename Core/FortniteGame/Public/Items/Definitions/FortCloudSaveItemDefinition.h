#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortMetadataItemDefinition.h"

class UFortCloudSaveItemDefinition : public UFortMetadataItemDefinition {
public:
	DefineUnrealClass(UFortCloudSaveItemDefinition);

	DefineUProperty(int32, ContentVersion);
};
