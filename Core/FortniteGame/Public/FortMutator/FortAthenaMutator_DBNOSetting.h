#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_DBNOSetting : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_DBNOSetting);

	DefineUProperty(EDBNOMutatorType, DBNOSetting);
};
