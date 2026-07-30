#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortEventItemDefinitionBase : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortEventItemDefinitionBase);

	DefineUProperty(int32, EventPriority);
	DefineUProperty(bool, bActivateByDefault);
	DefineUProperty(FName, RequiredEventTag);
	DefineUProperty(FName, DisallowedEventTag);
};
