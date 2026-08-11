#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/CoreNet.h"

class UChannel;
class UNetConnection;

class alignas(16) FOutBunch : public FNetBitWriter
{
private:
	uint8 Padding[0x300];

public:
	FOutBunch(class UChannel* InChannel, bool bClose);

public:
	bool GetbDormant() {
		if (Version::Engine_Version == 4.16) {
			return *((bool*)(uintptr_t)this + 0xE6);
		}

		return 0;
	}

	bool GetbReliable() {
		if (Version::Engine_Version == 4.16) {
			return *((bool*)(uintptr_t)this + 0xE8);
		}

		return 0;
	}

	void SetbDormant(bool Value) {
		if (Version::Engine_Version == 4.16) {
			*((bool*)(uintptr_t)this + 0xE6) = Value;
		}
	}

	void SetbReliable(bool Value) {
		if (Version::Engine_Version == 4.16) {
			*((bool*)(uintptr_t)this + 0xE8) = Value;
		}
	}

	bool MarkReliable(void* OwnerChannel, int32 OwnerChIndex)
	{
		uintptr_t ChannelOff = 0;
		for (uintptr_t Off = 0x08; Off <= 0x200; Off += sizeof(void*))
		{
			if (*reinterpret_cast<void* const*>((uintptr_t)this + Off) == OwnerChannel)
			{
				ChannelOff = Off;
				break;
			}
		}

		if (ChannelOff)
		{
			const int32 ChIndexInBunch = *reinterpret_cast<int32*>((uintptr_t)this + ChannelOff + 0x10);
			uint8* Flags = reinterpret_cast<uint8*>((uintptr_t)this + ChannelOff + 0x28);

			if (ChIndexInBunch == OwnerChIndex && *Flags == 0x04)
			{
				*Flags |= 0x20;
				return true;
			}
		}

		if (Version::Engine_Version == 4.16)
		{
			if (*reinterpret_cast<uint8*>((uintptr_t)this + 0xE5) == 1)
			{
				*reinterpret_cast<uint8*>((uintptr_t)this + 0xE8) = 1;
				return true;
			}
		}

		return false;
	}
};
