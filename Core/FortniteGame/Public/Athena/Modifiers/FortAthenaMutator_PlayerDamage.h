#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"

class AFortAthenaMutator_PlayerDamage : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_PlayerDamage);

	DefineUProperty(FGameplayTagQuery, DamageTagQuery);
	DefineUProperty(float, DamageMultiplier);
};
