#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_MinigameDamageSupression : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_MinigameDamageSupression);

	DefineBitfieldUProperty(bPreventPreGameDamage);
	DefineBitfieldUProperty(bPreventPostGameDamage);
};
