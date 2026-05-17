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

	static inline void (*OnRep_CurrentBuildingLevelOG)(ABuildingActor* This);
	static void OnRep_CurrentBuildingLevel(ABuildingActor* This);

	static inline void (*PlacedByPlacementToolOG)(ABuildingActor* This);
	static void PlacedByPlacementTool(ABuildingActor* This);

	float GetHealth() const;
public:
	static void Hook() {
		MH_CreateHook((LPVOID)(ImageBase + Finder::FindABuildingActor_OnDamageServer()), OnDamageServer, (LPVOID*)&OnDamageServerOG);

		HookEveryVTable(
			ABuildingActor::StaticClass(),
			ABuildingActor::StaticClass()->GetFunction("Function /Script/FortniteGame.BuildingActor.OnRep_CurrentBuildingLevel"),
			OnRep_CurrentBuildingLevel,
			(LPVOID*)&OnRep_CurrentBuildingLevelOG
		);

		HookEveryVTable(
			ABuildingActor::StaticClass(),
			ABuildingActor::StaticClass()->GetFunction("Function /Script/FortniteGame.BuildingActor.PlacedByPlacementTool"),
			PlacedByPlacementTool,
			(LPVOID*)&PlacedByPlacementToolOG
		);

		Log("ABuildingActor Hooked!");
	}
};