#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Engine/Public/Net/DataBunch.h"

class FInBunch;
class FOutBunch;
class UNetConnection;

enum EChannelType
{
    CHTYPE_None = 0,  // Invalid type.
    CHTYPE_Control = 1,  // Connection control.
    CHTYPE_Actor = 2,  // Actor-update channel.
    CHTYPE_File = 3,  // Binary file transfer.
    CHTYPE_Voice = 4,  // VoIP data channel
    CHTYPE_MAX = 8,  // Maximum.
};

class UChannel : public UObject {
public:
    DefineUnrealClass(UChannel);

    DefineUProperty(UNetConnection*, Connection);

    DefineCustomProperty(int32, NumInRec, ServerOffsets::UChannel__NumInRec);
    DefineCustomProperty(int32, NumOutRec, ServerOffsets::UChannel__NumOutRec);
public:
    int32 IsNetReady(bool Saturate);

    FPacketIdRange SendBunch(FOutBunch* Bunch, bool Merge);

	void Close();

	void StartBecomingDormant();
};