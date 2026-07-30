#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

#include "FortniteGame/Public/Player/AthenaQuickChatBank.h"

struct FAthenaQuickChatActiveEntry {
public:
	DefineUnrealStruct(FAthenaQuickChatActiveEntry);

	DefineStructProperty(TWeakObjectPtr<UAthenaQuickChatBank>, Bank);
	DefineStructProperty(UObject*, ContextObject);
	DefineStructProperty(int16, ContextValue);
	DefineStructProperty(int8, Index);
public:
	uint8 Padding[0x18];
public:
	UAthenaQuickChatBank* GetBank() const {
		UStruct* Struct = StaticStruct();
		if (!Struct) {
			Log("FAthenaQuickChatActiveEntry::GetBank: AthenaQuickChatActiveEntry struct not found!");
			return nullptr;
		}

		static UProperty* BankProperty = Struct->FindPropertyByName("Bank");
		if (!BankProperty) {
			Log("FAthenaQuickChatActiveEntry::GetBank: Bank property not found!");
			return nullptr;
		}

		// Only a plain object property stores the pointer inline. Bank is a TWeakObjectPtr, which holds an
		// object index and a serial number instead, so reading those 8 bytes as a pointer gives a wild
		// address made of the two halves.
		UObject* Object = nullptr;
		if (BankProperty->IsA(CASTCLASS_FObjectProperty)) {
			Object = GetFromOffset<UObject*>(this, BankProperty->Offset_Internal);
		}
		else {
			Object = Bank.Get();
		}

		// The entry is filled in from a client RPC, so the bank is whatever object reference the client
		// felt like sending. Anything that is not really a bank would have its memory read as ChatOptions.
		return Object ? Object->Cast<UAthenaQuickChatBank>() : nullptr;
	}
};