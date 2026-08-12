#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Info.h"

class APlayerState : public AInfo {
public:
	DefineUnrealClass(APlayerState);

	DefineBitfieldUProperty(bOnlySpectator);
	DefineUProperty(uint8, Ping);
	DefineUProperty(FString, PlayerName);
	DefineUProperty(FString, PlayerNamePrivate);
	DefineBitfieldUProperty(bIsABot);
	DefineUProperty(float, Score);
	DefineUProperty(int32, PlayerId);
	DefineUProperty(int32, StartTime);
	DefineBitfieldUProperty(bIsSpectator);
	DefineBitfieldUProperty(bIsInactive);
	DefineBitfieldUProperty(bFromPreviousLevel);
public:
	FString GetPlayerName() const;

	FORCEINLINE float GetScore() const { return Score; }
	FORCEINLINE int32 GetPlayerId() const { return PlayerId; }
	FORCEINLINE bool IsSpectator() const { return bIsSpectator; }
	FORCEINLINE bool IsOnlyASpectator() const { return bOnlySpectator; }
	FORCEINLINE bool IsABot() const { return bIsABot; }
	FORCEINLINE bool IsInactive() const { return bIsInactive; }
};