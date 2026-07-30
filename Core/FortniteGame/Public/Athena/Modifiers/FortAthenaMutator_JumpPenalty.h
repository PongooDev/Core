#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_JumpPenalty : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_JumpPenalty);

	DefineUProperty(bool, bApplyJumpPenalty);
};
