#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_Flag : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_Flag);

	DefineUProperty(FText, GameMsgText_Intro);
};
