#pragma once
#include "pch.h"

template<class ObjectType>
class TSharedPtr {
public:
	FORCEINLINE ObjectType* Get() const
	{
		return Object;
	}

	FORCEINLINE const bool IsValid() const
	{
		return Object != nullptr;
	}

	FORCEINLINE ObjectType* operator->() const
	{
		return Object;
	}

	FORCEINLINE void Reset()
	{
		*this = TSharedPtr<ObjectType>();
	}
public:
	ObjectType* Object;
	void* ReferenceController;
};