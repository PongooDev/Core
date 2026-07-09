#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

void AController::Possess(APawn* InPawn)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Possess");

	return Call(Func, InPawn);
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