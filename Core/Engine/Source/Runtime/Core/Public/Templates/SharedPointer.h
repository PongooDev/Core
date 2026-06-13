#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

struct FNullTag {};

struct TReferenceControllerBase {
	int32 SharedReferenceCount{ 1 };
	int32 WeakReferenceCount{ 1 };
};

template <class ObjectType>
struct TSharedRef {
	ObjectType* Object;

	TReferenceControllerBase* SharedReferenceCount;
};

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

	FORCEINLINE TSharedPtr(FNullTag* = nullptr)
		: Object(nullptr)
		, SharedReferenceCount()
	{
	}

	TSharedPtr(const TSharedRef<ObjectType>& Ref)
	{
		Object = Ref.Object;
	}
public:
	ObjectType* Object;
	int32 SharedReferenceCount;
	int32 WeakReferenceCount;
};