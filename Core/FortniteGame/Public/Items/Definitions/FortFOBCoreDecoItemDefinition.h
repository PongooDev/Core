#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Internationalization/Text.h"
#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Weapons/FortDecoItemDefinition.h"

class UFortFOBCoreDecoItemDefinition : public UFortDecoItemDefinition {
public:
	DefineUnrealClass(UFortFOBCoreDecoItemDefinition);

	DefineUProperty(FText, ExtendedDescription);
	DefineUProperty(FText, FlavorText);
	DefineUProperty(FGuid, SaveFilenameGUID);
};
