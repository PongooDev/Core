#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"

#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"

enum class EFortResourceType : uint8 {
	Wood,
	Stone,
	Metal,
	Permanite,
	None,
};

class UFortResourceItemDefinition final : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortResourceItemDefinition);
};
