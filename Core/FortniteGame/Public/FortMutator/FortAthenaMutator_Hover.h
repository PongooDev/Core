#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Hover : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Hover);

	DefineUProperty(FAthenaGameMessageData, GameMsg_Intro);
};
