#pragma once
#include "pch.h"

#include "FortniteGame/Public/Abilities/FortAttributeSet.h"

class UFortHealthSet : public UFortAttributeSet {
public:
	DefineUnrealClass(UFortHealthSet);

	DefineUProperty(FFortGameplayAttributeData, Health);
	DefineUProperty(FFortGameplayAttributeData, MaxHealth);
	DefineUProperty(FFortGameplayAttributeData, CurrentShield);
	DefineUProperty(FFortGameplayAttributeData, Shield);
public:
	void OnRep_CurrentShield();
	void OnRep_Health();
	void OnRep_MaxHealth();
	void OnRep_Shield();
};