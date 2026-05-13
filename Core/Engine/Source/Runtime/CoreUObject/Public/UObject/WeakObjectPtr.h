#pragma once
#include "pch.h"

#include "Object.h"
#include "UObjectArray.h"

class FWeakObjectPtr
{
public:
	FWeakObjectPtr(int32 Index = 0, int32 SerialNumber = 0)
		: ObjectIndex(Index)
		, ObjectSerialNumber(SerialNumber)
	{
	}

	FWeakObjectPtr(const UObject* Object)
	{
		if (Object)
		{
			ObjectIndex = Object->InternalIndex;
			auto Item = FUObjectArray::IndexToObject(Object->InternalIndex);
			ObjectSerialNumber = Item->SerialNumber;
		}
		else
		{
			ObjectIndex = INDEX_NONE;
			ObjectSerialNumber = 0;
		}
	}

	FORCEINLINE FWeakObjectPtr(const FWeakObjectPtr& Other)
	{
		ObjectIndex = Other.ObjectIndex;
		ObjectSerialNumber = Other.ObjectSerialNumber;
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
		if (!this)
			return nullptr;

		if (ObjectIndex < 0 || ObjectSerialNumber == 0)
			return nullptr;

		auto Item = FUObjectArray::IndexToObject(ObjectIndex);

		if (!Item || Item->SerialNumber != ObjectSerialNumber)
			return nullptr;

		return (UObject*)Item->Object;
	}

	const UObject* operator->() const
	{
		return Get();
	}

	bool operator==(const FWeakObjectPtr& Other) const
	{
		return ObjectIndex == Other.ObjectIndex;
	}

	bool operator!=(const FWeakObjectPtr& Other) const
	{
		return ObjectIndex != Other.ObjectIndex;
	}

	bool operator==(const class UObject* Other) const
	{
		return ObjectIndex == Other->InternalIndex;
	}

	bool operator!=(const class UObject* Other) const
	{
		return ObjectIndex != Other->InternalIndex;
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
public:
	UEType* Get() const
	{
		return (UEType*)FWeakObjectPtr::Get();
	}
};