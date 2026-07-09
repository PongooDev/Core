#include "pch.h"
#include "FortniteGame/Public/Athena/FortAthenaMapInfo.h"

#include "FortniteGame/Public/FortGameMode/FortGameModeAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerZone.h"
#include "FortniteGame/Public/FortPlayerState/FortPlayerStateZone.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySet.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"
#include "FortniteGame/Public/AI/FortAIGoalManager.h"
#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortGameState/FortGameStateZone.h"
#include "FortniteGame/Public/FortManager/FortMissionManager.h"
#include "FortniteGame/Public/Kismet/FortMissionLibrary.h"
#include "FortniteGame/Public/FortAbility/FortAbilitySystemComponent.h"
#include "FortniteGame/Public/BuildingActor/BuildingItemCollectorActor.h"
#include "FortniteGame/Public/FortGameState/FortGameStateAthena.h"

void AFortAthenaMapInfo::SpawnLlamas()
{
	int32 SpawnedLlamas = 0;

	UWorld* World = UWorld::GetWorld();
	if (!World || !World->AuthorityGameMode)
	{
		Log("AFortAthenaMapInfo::SpawnLlamas: World or AuthorityGameMode is null.");
		return;
	}

	AFortGameModeAthena* GameMode = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	if (!GameMode || !GameMode->GameState)
	{
		Log("AFortAthenaMapInfo::SpawnLlamas: GameMode or GameState is null.");
		return;
	}

	AFortGameStateAthena* GameState = GameMode->GameState->Cast<AFortGameStateAthena>();
	if (!GameState || !GameState->MapInfo)
	{
		Log("AFortAthenaMapInfo::SpawnLlamas: GameStateAthena or MapInfo is null.");
		return;
	}

	const int32 LlamaMin = (int32)GameState->MapInfo->LlamaQuantityMin.Evaluate();
	const int32 LlamaMax = (int32)GameState->MapInfo->LlamaQuantityMax.Evaluate();

	if (LlamaMax < LlamaMin)
	{
		Log("AFortAthenaMapInfo::SpawnLlamas: Invalid llama quantity range.");
		return;
	}

	const int32 LlamaCount = UKismetMathLibrary::RandomIntegerInRange(LlamaMin, LlamaMax);
	if (LlamaCount <= 0)
	{
		Log("AFortAthenaMapInfo::SpawnLlamas: LlamaCount <= 0.");
		return;
	}

	FFortSafeZoneDefinition* SafeZoneDef = nullptr;
	if (_HasSafeZoneDefinition())
	{
		SafeZoneDef = &SafeZoneDefinition;
	}
	if (!SafeZoneDef && SafeZoneDefinitions.Num() > 0)
	{
		SafeZoneDef = &SafeZoneDefinitions[0];
	}

	float Radius = SafeZoneDef ? SafeZoneDef->Radius.Evaluate(0) : 120000.0f;
	if (Radius <= 0.0f)
	{
		Radius = 120000.0f;
	}

	FVector Center = GetMapCenter();
	Center.Z = 10000.0f;

	for (int32 i = 0; i < LlamaCount; ++i)
	{
		const FVector Loc = PickSupplyDropLocation(Center, Radius, 0, -1, -1);
		const bool bValidLoc = (Loc.X != 0.0f || Loc.Y != 0.0f || Loc.Z != 0.0f);
		if (!bValidLoc)
			continue;

		FRotator Rot{};
		Rot.Yaw = UKismetMathLibrary::RandomFloatInRange(0.0f, 360.0f);

		AActor* NewLlamaActor = World->SpawnActorUnfinished(LlamaClass.Class, Loc, Rot);
		if (!NewLlamaActor)
		{
			Log("AFortAthenaMapInfo::SpawnLlamas: Failed SpawnActorUnfinished.");
			continue;
		}

		AFortAthenaSupplyDrop* NewLlama = NewLlamaActor->Cast<AFortAthenaSupplyDrop>();
		if (!NewLlama)
		{
			Log("AFortAthenaMapInfo::SpawnLlamas: Failed cast to AFortAthenaSupplyDrop.");
			continue;
		}

		const FVector GroundLoc = NewLlama->FindGroundLocationAt(Loc);
		World->FinishSpawnActor(NewLlama, GroundLoc, Rot);
		++SpawnedLlamas;
	}

	Log("AFortAthenaMapInfo::SpawnLlamas: Spawned " + std::to_string(SpawnedLlamas) +
		" of " + std::to_string(LlamaCount) + " llamas.");
}

FVector AFortAthenaMapInfo::PickSupplyDropLocation(FVector& CenterLocation, float MaxRadius, bool bAvoidWater, float TraceStartZ, float TraceEndZ) {
	FVector ResultLocation;
	FVector* (*PickSupplyDropLocationInternal)(AFortAthenaMapInfo*, FVector* result, FVector&, float, bool, float, float) = decltype(PickSupplyDropLocationInternal)(ImageBase + Finder::FindAFortAthenaMapInfo_PickSupplyDropLocation());
	PickSupplyDropLocationInternal(this, &ResultLocation, CenterLocation, MaxRadius, bAvoidWater, TraceStartZ, TraceEndZ);
	return ResultLocation;
}

FVector AFortAthenaMapInfo::GetMapCenter() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetMapCenter");

	if (!Func) 
	{
		return FVector(0, 0, 0);
	}

	return const_cast<AFortAthenaMapInfo*>(this)->Call<FVector>(Func);
}