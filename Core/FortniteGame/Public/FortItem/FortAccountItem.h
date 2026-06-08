#pragma once
#include "pch.h"

#include "FortItem.h"

class UFortAccountItemDefinition;

class UFortAccountItem : public UFortItem {
public:
	DefineUnrealClass(UFortAccountItem);

	DefineUProperty(UFortAccountItemDefinition*, ItemDefinition);
	DefineUProperty(FString, TemplateId);
};