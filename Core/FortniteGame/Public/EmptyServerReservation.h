#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/OnlineReplStructs.h"

struct FEmptyServerReservation {
public:
	DefineUnrealStruct(FEmptyServerReservation);

	DefineStructProperty(int32, PlaylistId);
	DefineStructProperty(FString, ZoneInstanceId);
	DefineStructProperty(FString, WUID);
	DefineStructProperty(FUniqueNetIdRepl, WorldDataOwner);
	DefineStructProperty(bool, bMakePrivate);
	DefineStructProperty(uint8, MatchmakingPool);
public:
	uint8 Padding[0x58];
};