#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Actor.h"
#include "Engine/Source/Runtime/GameplayTags/Classes/GameplayTagContainer.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/NoExportTypes.h"
#include "Engine/Plugins/Runtime/GameplayAbilities/Source/GameplayAbilities/Public/GameplayEffectTypes.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/HitResult.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetMathLibrary.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"

class ABuildingActor;
class ABuildingSMActor;
class AFortPlayerController;
class AFortPlayerStateZone;
class UFortWorldItemDefinition;
class UFortBuildingActorSet;
class USceneComponent;

struct FBuildingClassData
{
public:
	TSubclassOf<ABuildingActor> BuildingClass;
	int32 PreviousBuildingLevel;
	int32 UpgradeLevel;
};

class ABuildingActor : public AActor {
public:
	DefineUnrealClass(ABuildingActor);

	DefineBitfieldUProperty(bPlayerPlaced);

	DefineBitfieldUProperty(bDestroyed);

	DefineUProperty(uint8, Team);
	DefineUProperty(uint8, TeamIndex);

	DefineUProperty(bool, bAllowInteract);

	DefineUProperty(UFortBuildingActorSet*, BuildingAttributeSet);

	DefineUProperty(int32, CurrentBuildingLevel);
public:
	void InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController, bool bUsePlayerBuildAnimations, ABuildingActor* ReplacedBuilding);

	float GetHealthPercent() const;

	float GetMaxHealth() const;

	static inline void (*OnDamageServerOG)(ABuildingActor* This, float Damage, const FGameplayTagContainer& DamageTags, const FVector& Momentum, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, const FGameplayEffectContextHandle& EffectContext);
	static void OnDamageServer(ABuildingActor* This, float Damage, const FGameplayTagContainer& DamageTags, const FVector& Momentum, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, const FGameplayEffectContextHandle& EffectContext);

	float GetHealth() const;

	void OnRep_CurrentBuildingLevel();

	void OnRep_Team();
	void OnRep_TeamVFT();
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindABuildingActor_OnDamageServer()), OnDamageServer, (LPVOID*)&OnDamageServerOG);

		Log("ABuildingActor Hooked!");
	}
};