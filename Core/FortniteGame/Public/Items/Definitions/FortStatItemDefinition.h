#pragma once
#include "pch.h"

#include "FortniteGame/Public/Items/Definitions/FortAccountItemDefinition.h"

class EFortStatType {
public:
	DefineUnrealEnum(EFortStatType);

	DefineEnumProperty(Fortitude);
	DefineEnumProperty(Offense);
	DefineEnumProperty(Resistance);
	DefineEnumProperty(Technology);
	DefineEnumProperty(Fortitude_Team);
	DefineEnumProperty(Offense_Team);
	DefineEnumProperty(Resistance_Team);
	DefineEnumProperty(Technology_Team);
	DefineEnumProperty(Invalid);
};

class UFortStatItemDefinition : public UFortAccountItemDefinition {
public:
	DefineUnrealClass(UFortStatItemDefinition);

	DefineUProperty(EFortStatType, Stat);
};
