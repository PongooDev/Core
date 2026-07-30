#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "FortniteGame/Public/Abilities/FortAttributeSet.h"

class UFortDamageSet : public UFortAttributeSet {
public:
	DefineUnrealClass(UFortDamageSet);

	DefineUProperty(FFortGameplayAttributeData, OutgoingBaseDamage);
	DefineUProperty(FFortGameplayAttributeData, OutgoingBaseEnvironmentalDamage);
	DefineUProperty(FFortGameplayAttributeData, OutgoingBaseImpactDamage);
	DefineUProperty(FFortGameplayAttributeData, OutgoingAbilityDamage);
	DefineUProperty(FFortGameplayAttributeData, OutgoingBaseAbilityDamageMultiplier);
	DefineUProperty(FFortGameplayAttributeData, ZoneCritMultiplier);
	DefineUProperty(FFortGameplayAttributeData, DiceCritChance);
	DefineUProperty(FFortGameplayAttributeData, DiceCritMultiplier);
	DefineUProperty(FFortGameplayAttributeData, KnockbackMagnitude);
	DefineUProperty(FFortGameplayAttributeData, KnockbackZAngle);
	DefineUProperty(FFortGameplayAttributeData, StunTime);
	DefineUProperty(FFortGameplayAttributeData, WeaponSkill);
	DefineUProperty(FFortGameplayAttributeData, CritRating);
	DefineUProperty(FFortGameplayAttributeData, MaxMeleeComboLevel);
	DefineUProperty(FFortGameplayAttributeData, ShieldDamageScale);
	DefineUProperty(FFortGameplayAttributeData, HealthDamageScale);
};
