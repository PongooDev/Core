#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Info.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

class AFortTeamInfo : public AInfo {
public:
	DefineUnrealClass(AFortTeamInfo);

	DefineUProperty(TArray<AController*>, TeamMembers);
	DefineUProperty(uint8, Team);
	DefineUProperty(FString, ChatRoomId);
};