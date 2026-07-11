#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

void AController::Possess(APawn* InPawn)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Possess");

	return Call(Func, InPawn);
}

void AController::PossessVFT(APawn* InPawn)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Possess");

	uintptr_t VFT = GetVTableIndex(Func);
	if (VFT == -1)
		return;

	void (*&PossessInternal)(AController*, APawn*) = decltype(PossessInternal)(VTable[VFT]);
	PossessInternal(this, InPawn);
}

void AController::UnPossess()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("UnPossess");

	return Call(Func);
}

void AController::UnPossessVFT()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("UnPossess");

	uintptr_t VFT = GetVTableIndex(Func);
	if (VFT == -1)
		return;

	void (*&UnPossessInternal)(AController*) = decltype(UnPossessInternal)(VTable[VFT]);
	UnPossessInternal(this);
}

APawn* AController::K2_GetPawn() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_GetPawn"));

	return const_cast<AController*>(this)->Call<APawn*>(Func);
}

AActor* AController::GetViewTarget() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetViewTarget");

	return const_cast<AController*>(this)->Call<AActor*>(Func);
}

void AController::InitPlayerState()
{
	void (*&InitPlayerStateInternal)(AController*) = decltype(InitPlayerStateInternal)(VTable[Finder::FindAController_InitPlayerStateVFT()]);
	InitPlayerStateInternal(this);
}

void AController::OnRep_Pawn()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Pawn");

	Call(Func);
}

void AController::OnRep_PlayerState()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_PlayerState");
	
	Call(Func);
}

FRotator AController::GetControlRotation() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("GetControlRotation");

	return const_cast<AController*>(this)->Call<FRotator>(Func);
}