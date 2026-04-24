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
    inline bool GetClosing() {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 1;
        }

		return (*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset) & (1 << BitIndex)) != 0;
    }

    inline void SetClosing(bool Value) {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 1;
        }
        
		uint8_t* BitfieldPtr = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset);
        if (Value) {
            *BitfieldPtr |= (1 << BitIndex);
        }
        else {
            *BitfieldPtr &= ~(1 << BitIndex);
		}
    }

    inline bool GetDormant()
    {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 2;
        }

		return (*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset) & (1 << BitIndex)) != 0;
    }

    inline void SetDormant(bool Value)
    {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 2;
        }

		uint8_t* BitfieldPtr = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset);
        if (Value) {
            *BitfieldPtr |= (1 << BitIndex);
        }
        else {
            *BitfieldPtr &= ~(1 << BitIndex);
        }
    }

    inline bool GetbPendingDormancy()
    {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 7;
        }

		return (*reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset) & (1 << BitIndex)) != 0;
    }

    inline void SetPendingDormancy(bool Value)
    {
        static uintptr_t BitfieldOffset = 0;
        static int32 BitIndex = 0;
        if (Version::Engine_Version == 4.16) {
            BitfieldOffset = 0x30;
            BitIndex = 7;
        }

		uint8_t* BitfieldPtr = reinterpret_cast<uint8_t*>(reinterpret_cast<uintptr_t>(this) + BitfieldOffset);
        if (Value) {
            *BitfieldPtr |= (1 << BitIndex);
        }
        else {
            *BitfieldPtr &= ~(1 << BitIndex);
		}
    }

public:
    int32 IsNetReady(bool Saturate);

    FPacketIdRange SendBunch(FOutBunch* Bunch, bool Merge);
};