#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"


class UFortGameplayModifierItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortGameplayModifierItemDefinition);

	DefineUProperty(TArray<TSoftClassPtr<UClass>>, Mutators);
};
