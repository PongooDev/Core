#include "pch.h"
#include "FortniteGame/Public/BuildingActor/BuildingGameplayActor.h"

#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawnAthena.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

AFortPickup* AFortAthenaSupplyDrop::SpawnPickup(UFortWorldItemDefinition* ItemDefinition, int32 NumberToSpawn, AFortPawn* TriggeringPawn, FVector& Position, FVector& Direction) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("AFortAthenaSupplyDrop::SpawnPickup: World is null!");
		return nullptr;
	}

	AFortPickup* Pickup = UFortKismetLibrary::K2_SpawnPickupInWorld(
		World,
		ItemDefinition,
		NumberToSpawn,
		Position,
		*FVector::Allocate(),
		-1,
		true,
		true,
		false,
		-1,
		EFortPickupSourceTypeFlag::GetOther(),
		EFortPickupSpawnSource::GetSupplyDrop(),
		nullptr,
		false
	);
	if (Pickup) {
		UFortWeaponItemDefinition* WeaponDef = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWeaponItemDefinition>();
		if (WeaponDef) {
			int32 Level = Pickup->PrimaryPickupItemEntry.Level;
			Pickup->PrimaryPickupItemEntry.LoadedAmmo = WeaponDef->GetClipSize(Level);
			Pickup->PrimaryPickupItemEntry.Durability = WeaponDef->GetDurability(Level);
			Pickup->PrimaryPickupItemEntry.bIsDirty = true;
			Pickup->PrimaryPickupItemEntry.ReplicationKey++;
			Pickup->OnRep_PrimaryPickupItemEntry();
		}
	}

	return Pickup;
}

void AFortAthenaSupplyDrop::execSpawnPickup(AFortAthenaSupplyDrop* Context, FFrame& Stack, AFortPickup** Result) {
    UFortWorldItemDefinition* ItemDefinition;
    int32 NumberToSpawn;
    AFortPawn* TriggeringPawn;
    FVector Position;
    FVector Direction;
    Stack.StepCompiledIn(&ItemDefinition);
    Stack.StepCompiledIn(&NumberToSpawn);
    Stack.StepCompiledIn(&TriggeringPawn);
    Stack.StepCompiledIn(&Position);
    Stack.StepCompiledIn(&Direction);
    Stack.IncrementCode();

	*Result = Context->SpawnPickup(ItemDefinition, NumberToSpawn, TriggeringPawn, Position, Direction);
}

FVector AFortAthenaSupplyDrop::FindGroundLocationAt(const FVector& InLocation)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("FindGroundLocationAt");

	if (!Func) {
		return FVector(0, 0, 0);
	}

	return const_cast<AFortAthenaSupplyDrop*>(this)->Call<FVector>(Func, InLocation);
}