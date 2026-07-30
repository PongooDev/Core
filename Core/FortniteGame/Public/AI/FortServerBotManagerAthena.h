#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector2D.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Templates/SubclassOf.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/SlateCore/Public/Styling/SlateBrush.h"

class AFortAthenaMutator_Bots;
class AFortGameModeAthena;
class AFortPlayerPawnAthena;

class UFortServerBotManagerAthena : public UObject {
public:
	DefineUnrealClass(UFortServerBotManagerAthena);

	DefineUProperty(TSubclassOf<AFortPlayerPawnAthena>, BotPlayerPawnClass);
	DefineUProperty(AFortGameModeAthena*, CachedGameMode);
	DefineUProperty(uint8, LastBotTeam);
	DefineUProperty(AFortAthenaMutator_Bots*, CachedBotMutator);
	DefineUProperty(bool, bAddBotOnMinimap);
	DefineUProperty(FSlateBrush, SpecialActorMinimapIconBrush);
	DefineUProperty(FVector2D, SpecialActorMinimapIconScale);
	DefineUProperty(FSlateBrush, SpecialActorCompassIconBrush);
	DefineUProperty(FVector2D, SpecialActorCompassIconScale);
};
