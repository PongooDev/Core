#pragma once
#include "pch.h"

#include "UObjectBase.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

struct FGuid;
class UPackage;
class UClass;

class UObjectBaseUtility : public UObjectBase {
public:
	FString GetPathName(const UObject* StopOuter = NULL) const;

	void GetPathName(const UObject* StopOuter, FString& ResultString) const;

	FORCEINLINE FString GetName() const
	{
		if (!this) {
			return "None";
		}

		if (!IsValidLowLevelFast()) {
			return "None";
		}

		return GetFName().ToString();
	}

	UPackage* GetOutermost() const;

	void* GetInterfaceAddress(UClass* InterfaceClass);
};