#include "pch.h"
#include "FortniteGame/Public/Pawns/FortPlayerPawn.h"

#include "FortniteGame/Public/Inventory/FortInventory.h"
#include "FortniteGame/Public/Items/FortItemEntry.h"
#include "FortniteGame/Public/Player/FortPlayerControllerAthena.h"
#include "FortniteGame/Public/Weapons/FortWeaponItemDefinition.h"
#include "FortniteGame/Public/Items/Definitions/FortGadgetItemDefinition.h"
#include "FortniteGame/Public/Weapons/FortWeapon.h"
#include "FortniteGame/Public/Items/FortPickup.h"
#include "FortniteGame/Public/Kismet/FortKismetLibrary.h"
#include "FortniteGame/Public/FortGameModeAthena.h"
#include "FortniteGame/Public/Player/FortPlayerStateAthena.h"
#include "FortniteGame/Public/Abilities/FortAbilitySystemComponent.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "FortniteGame/Public/Items/Definitions/FortWorldItemDefinition.h"
#include "FortniteGame/Public/Inventory/FortQuickBars.h"
#include "FortniteGame/Public/Items/Definitions/FortItemDefinition.h"

void AFortPlayerPawn::BeginSkydiving(bool bFromBus)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"BeginSkydiving"));

	return Call(Func, bFromBus);
}

void AFortPlayerPawn::ForceFinishIncomingPickups()
{
	void (*&ForceFinishIncomingPickupsInternal)(AFortPlayerPawn * This) = decltype(ForceFinishIncomingPickupsInternal)(VTable[Finder::FindAFortPlayerPawn_ForceFinishIncomingPickupsVFT()]);
	ForceFinishIncomingPickupsInternal(this);
}

void AFortPlayerPawn::ServerChoosePart(UCustomCharacterPart* ChosenCharacterPart, uint8 Part)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerChoosePart");

	return Call(Func, Part, ChosenCharacterPart);
}

UCustomCharacterPart* AFortPlayerPawn::GetPreviousCharacterPart(uint8 Part)
{
	static UProperty* Prop = nullptr;

	if (Prop == nullptr)
		Prop = GetClass()->FindPropertyByName("PreviousCharacterParts");

	if (!Prop || Part >= 6)
		return nullptr;

	return ((UCustomCharacterPart**)((uintptr_t)this + Prop->Offset_Internal))[Part];
}

void AFortPlayerPawn::RandomizeCharacter(const FString& GenderString)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("RandomizeCharacter");

	return Call(Func, GenderString);
}

void AFortPlayerPawn::ServerReviveFromDBNO(AFortPlayerPawn* This, AController* EventInstigator)
{
	ServerReviveFromDBNOOG(This, EventInstigator);
	if (!This->IsDBNO()) {
		return; // ServerReviveFromDBNO was probably not stripped in this version
	}

	AFortPlayerController* FortPlayerController = This->Controller->Cast<AFortPlayerController>();
	if (!FortPlayerController) {
		Log("ServerReviveFromDBNO: FortPlayerController is null!");
		return;
	}

	AFortPlayerControllerZone* FortPlayerControllerZone = FortPlayerController->Cast<AFortPlayerControllerZone>();

	AFortPlayerState* FortPlayerState = FortPlayerController->PlayerState->Cast<AFortPlayerState>();
	if (!FortPlayerState) {
		Log("ServerReviveFromDBNO: FortPlayerState is null!");
		return;
	}

	FortPlayerState->AbilitySystemComponent->EndDBNOAbilities();

	This->bIsDBNO = false;
	This->bPlayedDying = false;

	int32 ReviveHealth = 30;
	// change revive health variable to more proper value (datatables?)

	This->SetHealth(ReviveHealth);

	This->OnRep_IsDBNO();

	if (FortPlayerControllerZone) {
		FortPlayerControllerZone->ClientOnPawnRevived(EventInstigator);
	}
}

void AFortPlayerPawn::ServerHandlePickup(AFortPlayerPawn* This, AFortPickup* Pickup, float InFlyTime, FVector& InStartDirection, bool bPlayPickupSound) {
	UWorld* World = UWorld::GetWorld();
	if (!World) {
		Log("ServerHandlePickup: World is null!");
		return;
	}

	if (!This->IsAllowedToPickup()) {
		return;
	}

	AFortGameModeAthena* FortGameModeAthena = World->AuthorityGameMode->Cast<AFortGameModeAthena>();
	
	float PickupSpeedMultiplier = This->_HasPickupSpeedMultiplier() ? This->PickupSpeedMultiplier : (FortGameModeAthena ? 3 : 1);
	InFlyTime = InFlyTime / PickupSpeedMultiplier;
	
	if (Version::Fortnite_Version <= 1.81 && Version::Fortnite_Version != 1.10 && Version::Fortnite_Version != 1.11) {
		return ServerHandlePickupOG(This, Pickup, InFlyTime, InStartDirection, bPlayPickupSound);
	}

	if (!This || !Pickup) {
		Log("ServerHandlePickup: This or Pickup is null!");
		return;
	}
	if (Pickup->bPickedUp) {
		//Log("ServerHandlePickup: Pickup is already picked up!");
		return;
	}

	Pickup->SetPickupTarget(This, InFlyTime, InStartDirection, bPlayPickupSound);
}

void AFortPlayerPawn::execOnCapsuleBeginOverlap(AFortPlayerPawn* Context, FFrame& Stack) {
	static UFunction* execOnCapsuleBeginOverlapFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawnAthena.OnCapsuleBeginOverlap");
	if (!execOnCapsuleBeginOverlapFn) {
		execOnCapsuleBeginOverlapFn = StaticClass()->GetFunction("Function /Script/FortniteGame.FortPlayerPawn.OnCapsuleBeginOverlap");
	}
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

	/*if (OtherActor) {
		Log("OnCapsuleBeginOverlap: Overlapped with: " + OtherActor->GetName().ToString());
	}
	else {
		Log("OnCapsuleBeginOverlap: Overlapped with null actor!");
	}*/
}

void AFortPlayerPawn::TryToAutoPickup(AFortPickup* Pickup) {
	if (!Pickup) {
		Log("TryToAutoPickup: Pickup is null!");
		return;
	}

	if (Pickup->IsPendingKillPending()) {
		//Log("TryToAutoPickup: Pickup is pending kill!");
		return;
	}

	if (!Pickup->CheckForRePickup(this)) {
		//Log("TryToAutoPickup: CheckForRePickup failed!");
		return;
	}

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
	if (!isAutoPickupType) {
		//Log("TryToAutoPickupWeapon: ItemType is not auto pickup type!");
		return;
	}

	AFortPlayerController* FortPlayerController = Controller->Cast<AFortPlayerController>();
	if (!FortPlayerController)
		return;

	AFortInventory* Inventory = FortPlayerController->WorldInventory;
	if (!Inventory)
		return;

	if (ItemDefinition->GetQuickBarForItem() == EFortQuickBars::GetPrimary()) {
		FFortItemEntry* ItemEntry = Inventory->FindItemEntry(ItemDefinition);
		if (!ItemEntry) {
			//Log("TryToAutoPickupWeapon: No ItemEntry for Primary QuickBars");
			return;
		}
	}

	if (!Inventory->CanAddItemWithStacking(ItemDefinition)) {
		//Log("TryToAutoPickupWeapon: Cannot add item with stacking");
		return;
	}

	//Log("TryToAutoPickupWeapon: Auto-picking up item: " + ItemDefinition->GetName().ToString());
	FVector ZeroVector(0, 0, 0);
	ServerHandlePickup(this, Pickup, 1.0f, ZeroVector, true);
}

void AFortPlayerPawn::OnRep_IsDBNO()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_IsDBNO");

	if (!Func) {
		return;
	}

	return const_cast<AFortPlayerPawn*>(this)->Call<void>(Func);
}

void AFortPlayerPawn::OnRep_ZiplineState()
{
	void (*OnRep_ZiplineStateInternal)(AFortPlayerPawn*) = decltype(OnRep_ZiplineStateInternal)(ImageBase + Finder::FindAFortPlayerPawn_OnRep_ZiplineState());
	OnRep_ZiplineStateInternal(this);
}

void AFortPlayerPawn::ServerSendZiplineState(const FZiplinePawnState& InZiplineState)
{
	if (!_HasZiplineState()) {
		return;
	}

	FZiplinePawnState::Copy(&ZiplineState, &InZiplineState);
	OnRep_ZiplineState();

	if (InZiplineState.bJumped && CharacterMovement)
	{
		constexpr float JumpXYLimit = 750.f;
		constexpr float JumpZVelocity = 1200.f;

		const FVector& Velocity = CharacterMovement->Velocity;

		LaunchCharacterJump(FVector{
			FMath::Clamp(Velocity.X * -0.5f, -JumpXYLimit, JumpXYLimit),
			FMath::Clamp(Velocity.Y * -0.5f, -JumpXYLimit, JumpXYLimit),
			JumpZVelocity
		}, false, false, true, true);
	}
}

void AFortPlayerPawn::execServerSendZiplineState(AFortPlayerPawn* Context, FFrame& Stack)
{
	FZiplinePawnState& InZiplineState = Stack.StepCompiledInRef<FZiplinePawnState>();
	Stack.IncrementCode();

	Context->ServerSendZiplineState(InZiplineState);
}
