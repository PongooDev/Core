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
#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"
#include "FortniteGame/Public/FortPlaylist/FortPlaylistAthena.h"

void ABuildingActor::InitializeKismetSpawnedBuildingActor(ABuildingActor* BuildingOwner, AFortPlayerController* SpawningController, bool bUsePlayerBuildAnimations, ABuildingActor* ReplacedBuilding)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("InitializeKismetSpawnedBuildingActor");

	return const_cast<ABuildingActor*>(this)->Call<void>(Func, BuildingOwner, SpawningController, bUsePlayerBuildAnimations, ReplacedBuilding);
}

float ABuildingActor::GetHealthPercent() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetHealthPercent");

	return const_cast<ABuildingActor*>(this)->Call<float>(Func);
}

float ABuildingActor::GetMaxHealth() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMaxHealth");

	if (!Func) {
		return 0.0f;
	}

	return const_cast<ABuildingActor*>(this)->Call<float>(Func);
}

void ABuildingActor::OnDamageServer(ABuildingActor* This, float Damage, const FGameplayTagContainer& DamageTags, const FVector& Momentum, const FHitResult& HitInfo, AController* InstigatedBy, AActor* DamageCauser, const FGameplayEffectContextHandle& EffectContext) {
	if (!This || This->bDestroyed) return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ABuildingActor::OnDamageServer: World is null!");
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	AFortGameStateAthena* FortGameStateAthena = World->GameState->Cast<AFortGameStateAthena>();

	AFortPlayerController* PC = InstigatedBy->Cast<AFortPlayerController>();
	if (!PC) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	ABuildingSMActor* BuildingSMActor = This->Cast<ABuildingSMActor>();
	if (!BuildingSMActor) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	if (This->bPlayerPlaced || !BuildingSMActor->bAllowResourceDrop) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	if (This->GetHealth() == 1) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	AFortWeapon* Weapon = DamageCauser->Cast<AFortWeapon>();
	if (!Weapon) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	if (!Weapon->WeaponData || !Weapon->WeaponData->IsA(UFortWeaponMeleeItemDefinition::StaticClass())) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	UFortResourceItemDefinition* ResourceDef = UFortKismetLibrary::K2_GetResourceItemDefinition(BuildingSMActor->ResourceType);
	if (!ResourceDef) {
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
	}

	UCurveTable* ResourceRates = nullptr;

	if (FortGameModeAthena && FortGameStateAthena) {
		UFortPlaylistAthena* CurrentPlaylist = FortGameStateAthena->CurrentPlaylistData;
		if (!CurrentPlaylist) {
			CurrentPlaylist = FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist;
		}
		if (CurrentPlaylist) {
			ResourceRates = CurrentPlaylist->ResourceRates.Get();
		}
	}
	if (!ResourceRates)
		ResourceRates = StaticLoadObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");
	if (!ResourceRates)
		ResourceRates = StaticLoadObject<UCurveTable>("/Game/Balance/DataTables/ResourceRates.ResourceRates");

	//int ResourceCount = (Damage / (UKismetMathLibrary::RandomIntegerInRange(8, 16)));
	int ResourceCount = 0;
	if (BuildingSMActor->HasBuildingResourceAmountOverride()) {
		FCurveTableRowHandle& BuildingResourceAmountOverride = BuildingSMActor->BuildingResourceAmountOverride;

		if (BuildingResourceAmountOverride.RowName.ComparisonIndex > 0)
		{
			float Out = 0.f;
			EEvaluateCurveTableResult::Type OutCurveTableResult;

			UDataTableFunctionLibrary::EvaluateCurveTableRow(ResourceRates, BuildingResourceAmountOverride.RowName, 0.f, &OutCurveTableResult, &Out, FString());

			float RC = Out / (BuildingSMActor->GetMaxHealth() / Damage);

			ResourceCount = (int)round(RC);
		}
	}

	if (ResourceCount <= 0)
		return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);

	PC->ClientReportDamagedResourceBuilding(
		BuildingSMActor,
		BuildingSMActor->ResourceType,
		ResourceCount,
		This->bDestroyed,
		(Damage == 100.f)
	);

	PC->WorldInventory->AddItemAndHandleOverflow(ResourceDef, ResourceCount);

	//This->ForceNetUpdate();
	return OnDamageServerOG(This, Damage, DamageTags, Momentum, HitInfo, InstigatedBy, DamageCauser, EffectContext);
}

float ABuildingActor::GetHealth() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetHealth");

	return const_cast<ABuildingActor*>(this)->Call<float>(Func);
}

void ABuildingActor::OnRep_CurrentBuildingLevel()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentBuildingLevel");

	Call(Func);
}