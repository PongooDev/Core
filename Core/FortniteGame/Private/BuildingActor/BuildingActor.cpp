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

	static UCurveTable* ResourceRates = nullptr;
	if (FortGameModeAthena && FortGameStateAthena) {
		if (FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist) {
			ResourceRates = FortGameStateAthena->CurrentPlaylistInfo.BasePlaylist->ResourceRates.Get();
		}
	}
	if (!ResourceRates)
		ResourceRates = StaticLoadObject<UCurveTable>("/Game/Athena/Balance/DataTables/AthenaResourceRates.AthenaResourceRates");
	if (!ResourceRates) 
		ResourceRates = StaticLoadObject<UCurveTable>("/Game/Balance/DataTables/ResourceRates.ResourceRates");

	int ResourceCount = (Damage / (UKismetMathLibrary::RandomIntegerInRange(8, 16)));
	/*if (BuildingSMActor->HasBuildingResourceAmountOverride()) {
		FCurveTableRowHandle& BuildingResourceAmountOverride = BuildingSMActor->BuildingResourceAmountOverride;

		if (BuildingResourceAmountOverride.RowName.ComparisonIndex > 0)
		{
			float Out = 0.f;
			EEvaluateCurveTableResult::Type OutCurveTableResult;

			UDataTableFunctionLibrary::EvaluateCurveTableRow(ResourceRates, BuildingResourceAmountOverride.RowName, 0.f, &OutCurveTableResult, &Out, FString());

			float RC = Out / (BuildingSMActor->GetMaxHealth() / Damage);

			ResourceCount = (int)round(RC);
		}
	}*/

	PC->ClientReportDamagedResourceBuilding(
		BuildingSMActor,
		BuildingSMActor->ResourceType,
		ResourceCount,
		This->bDestroyed,
		(Damage == 100.f)
	);

	PC->WorldInventory->AddItemAndHandleOverflow(ResourceDef, ResourceCount);

	This->ForceNetUpdate();
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

float ABuildingActor::GetHealth() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetHealth");

	struct BuildingActor_GetHealth final
	{
	public:
		float ReturnValue;
	};

	BuildingActor_GetHealth Parms{};

	const_cast<ABuildingActor*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}