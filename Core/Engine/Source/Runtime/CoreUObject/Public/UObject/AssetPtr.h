#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/PersistentObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/Misc/StringAssetReference.h"

class FAssetPtr : public TPersistentObjectPtr<FStringAssetReference> {
public:
	/** Default constructor, will be null */
	FORCEINLINE FAssetPtr()
	{
	}

	/** Construct from another asset pointer */
	FORCEINLINE FAssetPtr(const FAssetPtr& Other)
	{
		(*this) = Other;
	}

	const UObject* InternalGet()
	{
		if (!this)
			return nullptr;

		auto Object = WeakPtr.Get();

		if (!Object)
		{
			const UObject* Ret = nullptr;

			if (!ObjectID.AssetLongPathname.IsEmpty() && ObjectID.AssetLongPathname.IsValid())
				WeakPtr = Ret = StaticLoadObject(ObjectID.AssetLongPathname);

			return Ret;
		}

		return Object;
	}

	const UObject* Get()
	{
		return InternalGet();
	}
};

template<class T>
class TAssetPtr : public FAssetPtr
{
public:
	/** Default constructor, will be null */
	FORCEINLINE TAssetPtr()
	{
	}


	/**
	 * Dereference the asset pointer.
	 *
	 * @return nullptr if this object is gone or the lazy pointer was null, otherwise a valid UObject pointer
	 */
	FORCEINLINE T* Get()
	{
		return (T*)InternalGet();
	}

	/** Dereference the asset pointer */
	FORCEINLINE T& operator*() const
	{
		return *InternalGet();
	}

	/** Dereference the asset pointer */
	FORCEINLINE T* operator->() const
	{
		return InternalGet();
	}

	/**
	 * Test if this points to a live UObject
	 *
	 * @return true if Get() would return a valid non-null pointer
	 */
	FORCEINLINE bool IsValid() const
	{
		// This does the runtime type check
		return InternalGet() != nullptr;
	}
};