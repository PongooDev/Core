#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Engine/Classes/Components/ActorComponent.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_DBNOSetting.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_InventoryOverride.h"
#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator_NamePlate.h"

class EFortMinigamePlayerSpawnLocationSetting {
public:
	DefineUnrealEnum(EFortMinigamePlayerSpawnLocationSetting);

	DefineEnumProperty(SpawnPads);
	DefineEnumProperty(Air);
	DefineEnumProperty(CurrentLocation);
};

class UFortTeamSettingsComponent : public UActorComponent {
public:
	DefineUnrealClass(UFortTeamSettingsComponent);

	DefineUProperty(uint8, TeamIndex);
	DefineUProperty(float, MaxHealth);
	DefineUProperty(float, StartingHealth);
	DefineUProperty(float, MaxShield);
	DefineUProperty(float, StartingShield);
	DefineUProperty(EDBNOMutatorType, DBNOSetting);
	DefineUProperty(float, RespawnHeight);
	DefineUProperty(float, RespawnTime);
	DefineUProperty(EFortMinigamePlayerSpawnLocationSetting, SpawnLocationSetting);
	DefineUProperty(bool, bBlockBuilding);
	DefineUProperty(bool, bInstantReload);
	DefineUProperty(bool, bInfiniteAmmo);
	DefineUProperty(bool, bInfiniteResources);
	DefineUProperty(EAthenaLootDropOverride, DropAllItemsOverride);
	DefineUProperty(int32, HealthSiphonValue);
	DefineUProperty(int32, WoodSiphonValue);
	DefineUProperty(int32, StoneSiphonValue);
	DefineUProperty(int32, MetalSiphonValue);
	DefineUProperty(float, FallDamageMultiplier);
	DefineUProperty(int32, GravityOverride);
	DefineUProperty(bool, bApplyJumpPenalty);
	DefineUProperty(bool, bGliderRedeploy);
	DefineUProperty(bool, bFlyEnabled);
	DefineUProperty(int32, PlayerLives);
	DefineUProperty(uint8, TeamToMoveToWhenOutOfSpawns);
	DefineUProperty(bool, bRunOutTimeToWin);
	DefineUProperty(EIndicatorDisplayMode, NameplateDisplayMode);
	DefineUProperty(TArray<FString>, EnabledSettings);
};
