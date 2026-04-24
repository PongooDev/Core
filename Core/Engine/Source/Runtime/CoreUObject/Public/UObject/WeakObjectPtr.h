#pragma once
#include "pch.h"

#include "Object.h"
#include "UObjectArray.h"

class FWeakObjectPtr
{
public:
	FORCEINLINE FWeakObjectPtr()
	{
		Reset();
	}

	FORCEINLINE FWeakObjectPtr(const class UObject* Object)
	{
		if (!Object)
		{
			Log("FWeakObjectPtr: Constructor called with null object pointer.");
			Reset();
			return;
		}

		ObjectIndex = Object->InternalIndex;

		FUObjectItem* item = FUObjectArray::IndexToObject(ObjectIndex);
		if (item)
		{
			ObjectSerialNumber = item->SerialNumber;
		}
		else
		{
			Log("FWeakObjectPtr: Failed to get FUObjectItem for object index " + std::to_string(ObjectIndex));
			ObjectSerialNumber = 0;
		}
	}

	FORCEINLINE FWeakObjectPtr(const FWeakObjectPtr& Other)
	{
		(*this) = Other;
	}

	FORCEINLINE void Reset()
	{
		ObjectIndex = INDEX_NONE;
		ObjectSerialNumber = 0;
	}

	FORCEINLINE void operator=(const FWeakObjectPtr& Other)
	{
		ObjectIndex = Other.ObjectIndex;
		ObjectSerialNumber = Other.ObjectSerialNumber;
	}

	FORCEINLINE bool HasSameIndexAndSerialNumber(const FWeakObjectPtr& Other) const
	{
		return ObjectIndex == Other.ObjectIndex && ObjectSerialNumber == Other.ObjectSerialNumber;
	}
public:
	int32 ObjectIndex;
	int32 ObjectSerialNumber;
public:
	UObject* Get() const
	{
		return (UObject*)FUObjectArray::IndexToObject(ObjectIndex)->Object;
	}
};

template<typename UEType>
class TWeakObjectPtr : public FWeakObjectPtr
{
public:
	TWeakObjectPtr() : FWeakObjectPtr()
	{
	}

	TWeakObjectPtr(const UEType* Object) : FWeakObjectPtr(Object)
	{
	}
public:
	FORCEINLINE bool operator==(const TWeakObjectPtr<UEType>& Other) const
	{
		return HasSameIndexAndSerialNumber(Other);
	}
	FORCEINLINE bool operator!=(const TWeakObjectPtr<UEType>& Other) const
	{
		return !HasSameIndexAndSerialNumber(Other);
	}
};