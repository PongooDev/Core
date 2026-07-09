#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

void APlayerController::ServerAcknowledgePossession(APawn* P)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAcknowledgePossession");

	Call(Func, P);
}

void APlayerController::SendClientAdjustment()
{
	void (*&SendClientAdjustmentInternal)(APlayerController*) = decltype(SendClientAdjustmentInternal)(VTable[Finder::FindAPlayerController_SendClientAdjustmentVFT()]);
	SendClientAdjustmentInternal(this);
}

void APlayerController::ClientMessage(const FString& S, FName Type, float MsgLifeTime)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientMessage"));

	Call(Func, S, Type, MsgLifeTime);
}

void APlayerController::ClientMessage(const FString& S)
{
	ClientMessage(S, UKismetStringLibrary::Conv_StringToName("None"), 0.f);
}

void APlayerController::ClientTeamMessage(APlayerState* SenderPlayerState, const FString& S, FName Type, float MsgLifeTime)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName("ClientTeamMessage"));

	Call(Func, SenderPlayerState, S, Type, MsgLifeTime);
}