#pragma once
#include "pch.h"

#include "FortItemDefinition.h"

class UFortWorldItemDefinition : public UFortItemDefinition {
public:
	DefineUnrealClass(UFortWorldItemDefinition);
public:
	bool ServerExecute(UFortItem* Item, AFortPlayerController* Instigator);
};

class UFortAmmoItemDefinition : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortAmmoItemDefinition);
};

class UFortResourceItemDefinition final : public UFortWorldItemDefinition {
public:
	DefineUnrealClass(UFortResourceItemDefinition);
};