#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class EFortAthenaMutator_VoiceChatChannelType {
public:
	DefineUnrealEnum(EFortAthenaMutator_VoiceChatChannelType);

	DefineEnumProperty(Default);
	DefineEnumProperty(None);
	DefineEnumProperty(SquadOnly);
	DefineEnumProperty(TeamOnly);
	DefineEnumProperty(WholeServer);
	DefineEnumProperty(ScopeOnly);
	DefineEnumProperty(ScopeSquadOnly);
	DefineEnumProperty(ScopeTeamOnly);
	DefineEnumProperty(EFortAthenaMutator_MAX);
};

class AFortAthenaMutator_VoiceChat : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_VoiceChat);

	DefineUProperty(EFortAthenaMutator_VoiceChatChannelType, ChannelType);
};
