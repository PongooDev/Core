#include "pch.h"
#include "FortniteGame/Public/FortPawn/FortPlayerPawn.h"

#include "FortniteGame/Public/FortInventory/FortInventory.h"
#include "FortniteGame/Public/FortItem/FortItemEntry.h"
#include "FortniteGame/Public/FortPlayerController/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/FortItemDefinition/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/FortWeapon/FortWeapon.h"
#include "FortniteGame/Public/FortPickup/FortPickup.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"

void AFortPlayerPawn::BeginSkydiving(bool bFromBus)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"BeginSkydiving"));

	struct FortPlayerPawn_BeginSkydiving final
	{
	public:
		bool bFromBus;
	};

	FortPlayerPawn_BeginSkydiving Parms{};

	Parms.bFromBus = bFromBus;

	ProcessEvent(Func, &Parms);
}

void AFortPlayerPawn::ForceFinishIncomingPickups()
{
	void (*&ForceFinishIncomingPickupsInternal)(AFortPlayerPawn * This) = decltype(ForceFinishIncomingPickupsInternal)(VTable[Finder::FindAFortPlayerPawn_ForceFinishIncomingPickupsVFT()]);
	ForceFinishIncomingPickupsInternal(this);
}

void AFortPlayerPawn::ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, uint8 Part)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"ServerChoosePart"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&ServerChoosePartInternal)(AFortPlayerPawn*, uint8, UCustomCharacterPart*) = decltype(ServerChoosePartInternal)(VTable[VTableIdx]);
		return ServerChoosePartInternal(this, Part, ChosenCharacterPart);
	}
}

void AFortPlayerPawn::RandomizeCharacter(const FString& GenderString)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RandomizeCharacter");

	struct FortPlayerPawn_RandomizeCharacter
	{
	public:
		FString GenderString;
	};

	FortPlayerPawn_RandomizeCharacter Parms{};

	Parms.GenderString = std::move(GenderString);

	ProcessEvent(Func, &Parms);
}

void AFortPlayerPawn::ServerReviveFromDBNO(AFortPlayerPawn* This, AController* EventInstigator)
{
	ServerReviveFromDBNOOG(This, EventInstigator);

	Log("ServerReviveFromDBNO Called!");
}

void AFortPlayerPawn::ServerHandlePickup(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound) {
	if (Version::Fortnite_Version <= 1.81) {
		return ServerHandlePickupOG(This, Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
	}

	if (!This || !Pickup) {
		Log("ServerHandlePickup: This or Pickup is null!");
		return;
	}
	if (Pickup->bPickedUp) {
		Log("ServerHandlePickup: Pickup is already picked up!");
		return;
	}

	Pickup->SetPickupTarget(This, InFlyTime / This->PickupSpeedMultiplier, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawn::execOnCapsuleBeginOverlap(AFortPlayerPawn* Context, FFrame& Stack) {
	static UFunction* execOnCapsuleBeginOverlapFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawnAthena.OnCapsuleBeginOverlap");
	if (!execOnCapsuleBeginOverlapFn) {
		Log("AFortPlayerPawn::execOnCapsuleBeginOverlap: Failed to find function!");
		return;
	}

	UPrimitiveComponent* OverlappedComp = nullptr;
	AFortPickup* OtherActor = nullptr;
	UPrimitiveComponent* OtherComp = nullptr;
	int32 OtherBodyIndex = 0;
	bool bFromSweep = false;
	FHitResult SweepResult;

	for (auto& Param : execOnCapsuleBeginOverlapFn->GetParams().NameOffsetMap)
	{
		std::string Name = Param.Name.ToString();
		if (Name == "OverlappedComp") {
			Stack.StepCompiledIn(&OverlappedComp);
		}
		else if (Name == "OtherActor") {
			Stack.StepCompiledIn(&OtherActor);
		}
		else if (Name == "OtherComp") {
			Stack.StepCompiledIn(&OtherComp);
		}
		else if (Name == "OtherBodyIndex") {
			Stack.StepCompiledIn(&OtherBodyIndex);
		}
		else if (Name == "bFromSweep") {
			Stack.StepCompiledIn(&bFromSweep);
		}
		else if (Name == "SweepResult") {
			Stack.StepCompiledIn(&SweepResult);
		}
		else {
			Log("AFortPlayerPawn::execOnCapsuleBeginOverlap: Unhandled parameter: " + Name);
		}
	}
	Stack.IncrementCode();

	Context->OnCapsuleBeginOverlap(OverlappedComp, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AFortPlayerPawn::OnCapsuleBeginOverlap(UPrimitiveComponent* OverlappedComp, AFortPickup* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, FHitResult& SweepResult) {
	if (OtherActor && OtherActor->Cast<AFortPickup>()) {
		TryToAutoPickup(OtherActor);
	}
}

void AFortPlayerPawn::TryToAutoPickup(AFortPickup* Pickup) {
	if (!Pickup) {
		return;
	}

	if (Pickup->IsPendingKillPending()) {
		return;
	}

	if (!Pickup->CheckForRePickup(this))
		return;

	TryToAutoPickupWeapon(Pickup);
}

void AFortPlayerPawn::TryToAutoPickupWeapon(AFortPickup* Pickup) {
	if (!Pickup) {
		Log("TryToAutoPickupWeapon: Pickup is null!");
		return;
	}

	UFortWorldItemDefinition* ItemDefinition = Pickup->PrimaryPickupItemEntry.ItemDefinition->Cast<UFortWorldItemDefinition>();
	if (!ItemDefinition) {
		Log("TryToAutoPickupWeapon: ItemDefinition is null!");
		return;
	}

	uint8 ItemType = ItemDefinition->ItemType;
	bool isAutoPickupType = ItemType >= EFortItemType::GetWorldResource() || ItemType <= EFortItemType::GetTrap()
		|| ItemType == EFortItemType::GetConsumable();
	if (!isAutoPickupType)
		return;

	AFortPlayerController* FortPlayerController = Controller->Cast<AFortPlayerController>();
	if (!FortPlayerController)
		return;

	AFortInventory* Inventory = FortPlayerController->WorldInventory;
	if (!Inventory)
		return;

	if (ItemDefinition->GetQuickBarForItem() == EFortQuickBars::GetPrimary()) {
		FFortItemEntry* ItemEntry = Inventory->FindItemEntry(ItemDefinition);
		if (!ItemEntry) {
			return;
		}
	}

	if (!Inventory->CanAddItemWithStacking(ItemDefinition)) {
		return;
	}

	FVector ZeroVector(0, 0, 0);
	ServerHandlePickup(this, Pickup, 1.0f, ZeroVector, true);
}