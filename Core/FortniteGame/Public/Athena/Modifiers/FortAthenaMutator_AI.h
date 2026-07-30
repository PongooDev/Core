#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortGameplayMutator.h"

class AFortAthenaMutator_AI : public AFortGameplayMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_AI);

	DefineUProperty(TArray<UFortMovementComp_AIChar*>, DeimosMoveComponents);
};
