#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/SoftObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortAccountBuffItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAccountBuffItemDefinition);

	DefineUProperty(TArray<TSoftClassPtr<UClass>>, BuffEffects);
};
