#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class UAthenaQuickChatBank;

struct FAthenaQuickChatActiveEntry {
public:
	DefineUnrealStruct(FAthenaQuickChatActiveEntry);

	DefineStructProperty(TWeakObjectPtr<UAthenaQuickChatBank>, Bank);
	DefineStructProperty(UObject*, ContextObject);
	DefineStructProperty(int16, ContextValue);
	DefineStructProperty(int8, Index);
public:
	UAthenaQuickChatBank* GetBank() const {
		static UProperty* BankProperty = StaticStruct()->FindPropertyByName("Bank");
		if (!BankProperty) {
			Log("FAthenaQuickChatActiveEntry::GetBank: Bank property not found!");
			return nullptr;
		}

		if (BankProperty->IsA(UObjectProperty::StaticClass())) {
			UObjectProperty* ObjectProp = (UObjectProperty*)BankProperty;
			return GetFromOffset<UAthenaQuickChatBank*>(this, ObjectProp->Offset_Internal);
		}

		return Bank.Get();
	}
};