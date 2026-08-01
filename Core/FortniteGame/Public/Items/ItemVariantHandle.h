#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Misc/Guid.h"

class UFortItemDefinition;

struct FItemVariantHandle
{
public:
	UFortItemDefinition* Item;
	FGuid ItemVariantGuid;
};
