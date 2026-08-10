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
    static FORCEINLINE uintptr_t FlagsOffset()
    {
        static uintptr_t Cached = 0;

        if (!Cached)
        {
            const uintptr_t ConnectionOffset = UChannel::StaticClass()->GetPropertyOffset("Connection");

            if (ConnectionOffset != (uintptr_t)-1 && ConnectionOffset > 0)
                Cached = ConnectionOffset + sizeof(void*);
        }

        return Cached;
    }

    FORCEINLINE bool GetChannelFlag(int32 BitIndex) const
    {
        const uintptr_t Offset = FlagsOffset();
        if (!Offset)
            return false;

        return (*reinterpret_cast<uint32*>((uintptr_t)this + Offset) & (1u << BitIndex)) != 0;
    }

    FORCEINLINE void SetChannelFlag(int32 BitIndex, bool bValue) const
    {
        const uintptr_t Offset = FlagsOffset();
        if (!Offset)
            return;

        uint32& Word = *reinterpret_cast<uint32*>((uintptr_t)this + Offset);
        if (bValue)
            Word |= (1u << BitIndex);
        else
            Word &= ~(1u << BitIndex);
    }

    FORCEINLINE bool _GetDormant() const { return GetChannelFlag(2); }
    FORCEINLINE void _SetDormant(bool bValue) { SetChannelFlag(2, bValue); }

    FORCEINLINE bool _GetbPendingDormancy() const { return GetChannelFlag(7); }
    FORCEINLINE void _SetbPendingDormancy(bool bValue) { SetChannelFlag(7, bValue); }

    FORCEINLINE bool _GetClosing() const { return GetChannelFlag(1); }
    FORCEINLINE bool _GetOpenAcked() const { return GetChannelFlag(0); }
    FORCEINLINE bool _GetBroken() const { return GetChannelFlag(5); }

    __declspec(property(get = _GetDormant, put = _SetDormant)) bool Dormant;
    __declspec(property(get = _GetbPendingDormancy, put = _SetbPendingDormancy)) bool bPendingDormancy;
    __declspec(property(get = _GetClosing)) bool Closing;
    __declspec(property(get = _GetOpenAcked)) bool OpenAcked;
    __declspec(property(get = _GetBroken)) bool Broken;

public:
    int32 IsNetReady(bool Saturate);

    FPacketIdRange SendBunch(FOutBunch* Bunch, bool Merge);

	void Close();

	void StartBecomingDormant();
};