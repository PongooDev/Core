#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_DBNOSetting : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_DBNOSetting);

	DefineUProperty(EDBNOMutatorType, DBNOSetting);
};
