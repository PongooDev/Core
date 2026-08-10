#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/ActorChannel.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/ChildConnection.h"
#include "Engine/Source/Runtime/Engine/Classes/Engine/NetDriver.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"
#include "Engine/Source/Runtime/Engine/Public/EngineLogs.h"

int32 UNetConnection::IsNetReady(bool Saturate)
{
	if (!this) {
		return 0;
	}

	int32(*&IsNetReadyInternal)(UNetConnection*, bool) = decltype(IsNetReadyInternal)(VTable[Finder::FindUNetConnection_IsNetReadyVFT()]);
	return IsNetReadyInternal(this, Saturate);
}

UChildConnection* UNetConnection::GetUChildConnection()
{
	return NULL;
}

bool UNetConnection::UsesChannelNames()
{
	static int32 Cached = -1;

	if (Cached < 0)
	{
		const uintptr_t Offset = UNetDriver::StaticClass()->GetPropertyOffset("ChannelDefinitions");
		Cached = (Offset != (uintptr_t)-1) ? 1 : 0;
	}

	return Cached == 1;
}

UChannel* UNetConnection::CreateChannelByName(const FName& ChName, uint32 CreateFlags, int32 ChIndex)
{
	UChannel* (*CreateChannelByNameInternal)(UNetConnection*, const FName&, uint32, int32) = decltype(CreateChannelByNameInternal)(ImageBase + Finder::FindUNetConnection_CreateChannel());
	return CreateChannelByNameInternal(this, ChName, CreateFlags, ChIndex);
}

UChannel* UNetConnection::CreateChannel(EChannelType ChType, bool bOpenedLocally, int32 ChIndex)
{
	if (UsesChannelNames())
	{
		return CreateChannelByName(ChannelTypeToName(ChType), bOpenedLocally ? 1u : 0u, ChIndex);
	}

	UChannel* (*CreateChannelInternal)(UNetConnection*, EChannelType, bool, int32) = decltype(CreateChannelInternal)(ImageBase + Finder::FindUNetConnection_CreateChannel());
	return CreateChannelInternal(this, ChType, bOpenedLocally, ChIndex);
}

const FName& UNetConnection::ChannelTypeToName(EChannelType ChType)
{
	static FName NameActor{};
	static FName NameControl{};
	static FName NameVoice{};
	static bool bResolved = false;

	if (!bResolved)
	{
		bResolved = true;
		NameActor = UKismetStringLibrary::Conv_StringToName(L"Actor");
		NameControl = UKismetStringLibrary::Conv_StringToName(L"Control");
		NameVoice = UKismetStringLibrary::Conv_StringToName(L"Voice");
	}

	switch (ChType)
	{
	case CHTYPE_Control: return NameControl;
	case CHTYPE_Voice:   return NameVoice;
	default:             return NameActor;
	}
}

void UNetConnection::CleanUp()
{
	void (*&CleanUpInternal)(UNetConnection*) = decltype(CleanUpInternal)(VTable[Finder::FindUNetConnection_CleanUpVFT()]);
	CleanUpInternal(this);
}

void UNetConnection::SendChallengeControlMessage(UNetConnection* This, void* Response)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bUseGameSessions && !Config.bSkipSessionValidation)
		return SendChallengeControlMessageOG(This, Response);

	if (Response)
	{
		uint8& ResponseCode = *(uint8*)Response;
		if (ResponseCode != 1)
		{
			ResponseCode = 1;
		}
	}

	return SendChallengeControlMessageOG(This, Response);
}

void UNetConnection::Hook()
{
	MH_CreateHook((LPVOID)(ImageBase + Finder::FindUNetConnection_SendChallengeControlMessage()), SendChallengeControlMessage, (LPVOID*)&SendChallengeControlMessageOG);

	Log("UNetConnection Hooked");
}
