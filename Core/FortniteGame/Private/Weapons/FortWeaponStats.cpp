#include "pch.h"
#include "FortniteGame/Public/Weapons/FortWeaponStats.h"

int32 FFortBaseWeaponStats::GetClipSize(int32 WeaponLevel)
{
    int32 levelDelta = WeaponLevel - BaseLevel;
    if (levelDelta < 0)
        levelDelta = 0;

    const float clipSizeMultiplier = 1.0f + levelDelta * ClipScale;

    return ClipSize * clipSizeMultiplier;
}

float FFortBaseWeaponStats::GetDurability(int32 WeaponLevel, uint8 Rarity) {
	float (*GetDurabilityInternal)(FFortBaseWeaponStats * This, float WeaponLevel, uint8 Rarity) = decltype(GetDurabilityInternal)(ImageBase + Finder::FindFFortBaseWeaponStats_GetDurability());
	return GetDurabilityInternal(this, WeaponLevel, Rarity);
}