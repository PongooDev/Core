#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/PlayerController.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/LocalPlayer.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

ULocalPlayer* APlayerController::GetLocalPlayer() const
{
	return ::Cast<ULocalPlayer>(Player);
}

void APlayerController::ServerAcknowledgePossession(APawn* P)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAcknowledgePossession");

	Call(Func, P);
}

void APlayerController::ServerAcknowledgePossessionVFT(APawn* P)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("ServerAcknowledgePossession");

	if (!Func) {
		return;
	}

	void (*&ServerAcknowledgePossessionInternal)(APlayerController*, APawn*) = decltype(ServerAcknowledgePossessionInternal)(VTable[Func->GetVTableIndex()]);
	return ServerAcknowledgePossessionInternal(this, P);
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

	UE_LOG(LogPlayerController, Log, TEXT("%s"), *S);

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