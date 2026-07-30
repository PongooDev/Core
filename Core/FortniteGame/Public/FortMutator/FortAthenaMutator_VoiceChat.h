#pragma once
#include "pch.h"

#include "FortniteGame/Public/FortMutator/MutatorTypes.h"

#include "FortAthenaMutator.h"

class AFortAthenaMutator_VoiceChat : public AFortAthenaMutator {
public:
	DefineUnrealClass(AFortAthenaMutator_VoiceChat);

	DefineUProperty(EFortAthenaMutator_VoiceChatChannelType, ChannelType);
};
