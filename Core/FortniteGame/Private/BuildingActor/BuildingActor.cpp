#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingActor.h"

#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/BuildingActor/BuildingSMActor.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortSet/FortBuildingActorSet.h"

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

float ABuildingActor::GetMaxHealth() const
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName("GetMaxHealth"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		float (*&GetMaxHealthInternal)(const ABuildingActor*) = decltype(GetMaxHealthInternal)(VTable[VTableIdx]);
		return GetMaxHealthInternal(this);
	}
}

void ABuildingActor::OnDamageServer(ABuildingActor* This, float Damage, const FGameplayTagContainer& DamageTags, const FVector& Momentum, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, const FGameplayEffectContextHandle& EffectContext) {
	if (!This) return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingActor::OnDamageServer: World is null!");
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	AFortPlayerController* PC = InstigatedBy->Cast<AFortPlayerController>();
	if (!PC) {
		//Log("ABuildingActor::OnDamageServer: InstigatedBy is not a FortPlayerController!");
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	AFortWeapon* Weapon = DamageCauser->Cast<AFortWeapon>();
	if (!Weapon) {
		//Log("ABuildingActor::OnDamageServer: DamageCauser is not a FortWeapon!");
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	if (This->bPlayerPlaced) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	if (!Weapon->WeaponData || !Weapon->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass())) {
		Log("ABuildingActor::OnDamageServer: WeaponData is null or not a melee weapon! WeaponData: " + std::string(Weapon->WeaponData ? Weapon->WeaponData->GetName().ToString() : "None"));
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	int MaterialCount = (Damage / (UKismetMathLibrary::RandomIntegerInRange(6, 12)));

	ABuildingSMActor* BuildingSMActor = This->Cast<ABuildingSMActor>();
	if (!BuildingSMActor) {
		Log("ABuildingActor::OnDamageServer: This is not a BuildingSMActor!");
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	UFortResourceItemDefinition* ResourceDef = UFortKismetLibrary::K2_GetResourceItemDefinition(BuildingSMActor->ResourceType);
	if (ResourceDef) {
		PC->ClientReportDamagedResourceBuilding(
			BuildingSMActor,
			BuildingSMActor->ResourceType,
			MaterialCount,
			This->bDestroyed,
			(Damage == 100.f)
		);

		PC->WorldInventory->AddItemAndHandleOverflow(ResourceDef, MaterialCount);
	}
	else {
		Log("ABuildingActor::OnDamageServer: Failed to get ResourceDef for ResourceType: " + std::to_string(BuildingSMActor->ResourceType));
	}

	return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

void ABuildingActor::OnRep_CurrentBuildingLevel(ABuildingActor* This) {
	OnRep_CurrentBuildingLevelOG(This);

	Log("OnRep_CurrentBuildingLevel Called!");
}

void ABuildingActor::PlacedByPlacementTool(ABuildingActor* This) {
	if (!This)
		return;
	
	if (Version::Fortnite_Version >= 1.8) {
		Log("PlacedByPlacementTool Called. Actor: " + std::string(This ? This->GetName().ToString() : "None"));

		if (This->BuildingAttributeSet) {
			This->BuildingAttributeSet->Health.CurrentValue;
		}
		else {
			This->GetMaxHealth();
		}
	}
	else {
		PlacedByPlacementToolOG(This);
	}
}