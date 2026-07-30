#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_JumpPenalty : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_JumpPenalty);

	DefineUProperty(bool, bApplyJumpPenalty);
};
