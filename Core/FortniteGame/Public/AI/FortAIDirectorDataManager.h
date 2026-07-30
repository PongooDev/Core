#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/CurveTable.h"

class UObject;

class EFortAIDirectorEvent {
public:
	DefineUnrealEnum(EFortAIDirectorEvent);

	DefineEnumProperty(PlayerAIEnemies);
	DefineEnumProperty(PlayerTakeDamage);
	DefineEnumProperty(PlayerHealth);
	DefineEnumProperty(PlayerDeath);
	DefineEnumProperty(PlayerLookAtAIEnemy);
	DefineEnumProperty(PlayerDamageAIEnemy);
	DefineEnumProperty(PlayerKillAIEnemy);
	DefineEnumProperty(PlayerHealingPotential);
	DefineEnumProperty(PlayerAmmoLight);
	DefineEnumProperty(PlayerAmmoMedium);
	DefineEnumProperty(PlayerAmmoHeavy);
	DefineEnumProperty(PlayerAmmoShells);
	DefineEnumProperty(PlayerAmmoEnergy);
	DefineEnumProperty(PlayerAINear);
	DefineEnumProperty(PlayerMovement);
	DefineEnumProperty(ObjectiveTakeDamage);
	DefineEnumProperty(ObjectiveHealth);
	DefineEnumProperty(ObjectiveDestroyed);
	DefineEnumProperty(TrapFired);
	DefineEnumProperty(TrapDamagedAIEnemy);
	DefineEnumProperty(ObjectivePathCost);
	DefineEnumProperty(PlayerPathCost);
	DefineEnumProperty(ObjectiveNearbyBuildingDamaged);
	DefineEnumProperty(Max_None);
};

class EFortAIDirectorEventContribution {
public:
	DefineUnrealEnum(EFortAIDirectorEventContribution);

	DefineEnumProperty(Increment);
	DefineEnumProperty(Set);
};

class EFortAIDirectorEventParticipant {
public:
	DefineUnrealEnum(EFortAIDirectorEventParticipant);

	DefineEnumProperty(Target);
	DefineEnumProperty(Source);
	DefineEnumProperty(Either);
	DefineEnumProperty(Max_None);
};

struct FAIDirectorEventData {
public:
	DefineUnrealStruct(FAIDirectorEventData);

	DefineStructProperty(EFortAIDirectorEvent, Event);
	DefineStructProperty(FCurveTableRowHandle, DataMax);
	DefineStructProperty(FCurveTableRowHandle, CoolDownRate);
	DefineStructProperty(EFortAIDirectorEventContribution, ContributionType);
	DefineStructProperty(EFortAIDirectorEventParticipant, OwnerParticipantType);
public:
	uint8 Padding[0x2A];
};

class EFortAIDirectorFactor {
public:
	DefineUnrealEnum(EFortAIDirectorFactor);

	DefineEnumProperty(PlayerDamageThreat);
	DefineEnumProperty(ObjectiveDamageThreat);
	DefineEnumProperty(ObjectivePathCost);
	DefineEnumProperty(PlayerPathCost);
	DefineEnumProperty(PlayerMovement);
	DefineEnumProperty(TrapsEffective);
	DefineEnumProperty(PlayerWander);
	DefineEnumProperty(NearbyEnemyPresence);
	DefineEnumProperty(OffensiveResources);
	DefineEnumProperty(DefensiveResources);
	DefineEnumProperty(Boredom);
	DefineEnumProperty(ArtilleryVulnerability);
	DefineEnumProperty(Max_None);
};

struct FFortAIDirectorFactorData {
public:
	DefineUnrealStruct(FFortAIDirectorFactorData);

	DefineStructProperty(EFortAIDirectorFactor, AIDirectorFactor);
	DefineStructProperty(float, MaxValue);
public:
	uint8 Padding[0x1C];
};

class AFortAIDirectorDataManager : public AActor {
public:
	DefineUnrealClass(AFortAIDirectorDataManager);

	DefineUProperty(UObject*, OwnerObject);
	DefineUProperty(TArray<FAIDirectorEventData>, EventsToTrack);
	DefineUProperty(TArray<FFortAIDirectorFactorData>, FactorsToTrack);
	DefineUProperty(TArray<EFortAIDirectorFactor>, FactorsBeingTracked);
};
