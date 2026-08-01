#include "pch.h"
#include "FortniteGame/Public/Items/Definitions/FortGadgetItemDefinition.h"

#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortDecoItemDefinition.h"

UFortWeaponItemDefinition* UFortGadgetItemDefinition::GetGadgetWeapon()
{
	if (_HasWeaponItemDefinition()) {
		if (UFortWeaponItemDefinition* Weapon = WeaponItemDefinition.Get())
			return Weapon;
	}

	if (_HasDecoItemDefinition())
		return DecoItemDefinition.Get();

	return nullptr;
}
