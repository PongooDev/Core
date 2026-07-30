#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortPersistableItemDefinition.h"

class UFortProfileItemDefinition : public UFortPersistableItemDefinition {
public:
	DefineUnrealClass(UFortProfileItemDefinition);

	DefineUProperty(FString, GrantToProfileType);
};
