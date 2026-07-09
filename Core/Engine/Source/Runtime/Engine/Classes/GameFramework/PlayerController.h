#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"
#include "Engine/Source/Runtime/Core/Public/Math/Rotator.h"

class ACameraActor;
class AHUD;
class APawn;
class APlayerCameraManager;
class ASpectatorPawn;
class FDebugDisplayInfo;
class UActorChannel;
class UCheatManager;
class UGameViewportClient;
class UInterpTrackInstDirector;
class ULocalMessage;
class UNetConnection;
class UPlayer;
class UPrimitiveComponent;
struct FActiveHapticFeedbackEffect;
struct FCollisionQueryParams;
class APlayerCameraManager;

class APlayerController : public AController {
public:
	DefineUnrealClass(APlayerController);

	DefineUProperty(UCheatManager*, CheatManager);
	DefineUProperty(TSubclassOf<UCheatManager>, CheatClass);
	DefineUProperty(FVector, LastSpectatorSyncLocation);
	DefineUProperty(FRotator, LastSpectatorSyncRotation);
	DefineUProperty(APlayerCameraManager*, PlayerCameraManager);
public:
	static inline void (*ServerAcknowledgePossessionOG)(APlayerController* This, APawn* P);
	void ServerAcknowledgePossession(APawn* P);

	void SendClientAdjustment();

	void ClientMessage(const FString& S, FName Type, float MsgLifeTime);
	void ClientMessage(const FString& S);

	void ClientTeamMessage(APlayerState* SenderPlayerState, const FString& S, FName Type, float MsgLifeTime);

	static inline void (*GetPlayerViewPointOG)(APlayerController* This, FVector& out_Location, FRotator& out_Rotation);

	static void Hook() {
		CreateVTableOriginal(APlayerController::GetDefaultObj(), APlayerController::StaticClass()->GetFunction("Function /Script/Engine.PlayerController.ServerAcknowledgePossession"), (LPVOID*)&ServerAcknowledgePossessionOG);
		CreateVTableOriginal(APlayerController::GetDefaultObj(), Finder::FindAController_GetPlayerViewPointVFT(), (LPVOID*)&GetPlayerViewPointOG);

		Log("Hooked APlayerController");
	}
};