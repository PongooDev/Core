#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class UFortAccountBuffItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortAccountBuffItemDefinition);

	DefineUProperty(TArray<TSoftClassPtr<UClass>>, BuffEffects);
};
