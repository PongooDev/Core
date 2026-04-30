#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingActor.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"

void ABuildingActor::InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController, bool bUsePlayerBuildAnimations, ABuildingActor* ReplacedBuilding)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("InitializeKismetSpawnedBuildingActor"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&InitializeKismetSpawnedBuildingActorInternal)(ABuildingActor*, ABuildingActor*, AFortPlayerController*, bool, ABuildingActor*) = decltype(InitializeKismetSpawnedBuildingActorInternal)(VTable[VTableIdx]);
		return InitializeKismetSpawnedBuildingActorInternal(this, BuildingOwner, SpawningController, bUsePlayerBuildAnimations, ReplacedBuilding);
	}
}

float ABuildingActor::GetHealthPercent() const
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("GetHealthPercent"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		float (*&GetHealthPercentInternal)(const ABuildingActor*) = decltype(GetHealthPercentInternal)(VTable[VTableIdx]);
		return GetHealthPercentInternal(this);
	}
}

void ABuildingActor::OnDamageServer(ABuildingActor* This, float Damage, const FGameplayTagContainer& DamageTags, const FVector& Momentum, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, const FGameplayEffectContextHandle& EffectContext) {
	if (!This) return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}