#pragma once
#include "pch.h"


#include "FortniteGame/Public/Athena/Modifiers/FortAthenaMutator.h"
#include "FortniteGame/Public/FortEnums.h"

class AFortAthenaMutator_VoiceChat : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_VoiceChat);

	DefineUProperty(EFortAthenaMutator_VoiceChatChannelType, ChannelType);
};
