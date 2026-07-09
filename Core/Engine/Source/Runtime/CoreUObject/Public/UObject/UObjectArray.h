#pragma once
#include "pch.h"
#include "Core/Public/Version.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "ObjectMacros.h"
#include "UObjectBase.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

class UObject;
class UClass;
class UStruct;

struct FUObjectItem
{
	// Pointer to the allocated object
	UObjectBase* Object;
	// Internal flags
	int32 Flags;
	// UObject Owner Cluster Index
	int32 ClusterRootIndex;
	// Weak Object Pointer Serial number associated with the object
	int32 SerialNumber;

	FORCEINLINE void SetOwnerIndex(int32 OwnerIndex)
	{
		ClusterRootIndex = OwnerIndex;
	}

	FORCEINLINE int32 GetOwnerIndex() const
	{
		return ClusterRootIndex;
	}

	/** Encodes the cluster index in the ClusterRootIndex variable */
	FORCEINLINE void SetClusterIndex(int32 ClusterIndex)
	{
		ClusterRootIndex = -ClusterIndex - 1;
	}

	/** Decodes the cluster index from the ClusterRootIndex variable */
	FORCEINLINE int32 GetClusterIndex() const
	{
		checkSlow(ClusterRootIndex < 0);
		return -ClusterRootIndex - 1;
	}

	FORCEINLINE int32 GetSerialNumber() const
	{
		return SerialNumber;
	}

	FORCEINLINE void ResetSerialNumberAndFlags()
	{
		Flags = 0;
		ClusterRootIndex = 0;
		SerialNumber = 0;
	}
};

class FFixedUObjectArray
{
public:
	/** Static master table to chunks of pointers **/
	FUObjectItem* Objects;
	/** Number of elements we currently have **/
	int MaxElements;
	/** Current number of UObject slots */
	int NumElements;
public:
	/**
	* Expands the array so that Element[Index] is allocated. New pointers are all zero.
	* @param Index The Index of an element we want to be sure is allocated
	**/
	void PreAllocate(int32 InMaxElements)
	{
		check(!Objects);
		Objects = new FUObjectItem[InMaxElements];
		MaxElements = InMaxElements;
	}

	int32 AddSingle()
	{
		int32 Result = NumElements;
		check(Result == NumElements);
		++NumElements;
		check(Objects[Result].Object == nullptr);
		return Result;
	}

	int32 AddRange(int32 Count)
	{
		int32 Result = NumElements + Count - 1;
		check(Result == (NumElements + Count - 1));
		NumElements += Count;
		check(Objects[Result].Object == nullptr);
		return Result;
	}

	FUObjectItem const* GetObjectPtr(int32 Index) const
	{
		return &Objects[Index];
	}

	FUObjectItem* GetObjectPtr(int32 Index)
	{
		return &Objects[Index];
	}

	/**
	* Return the number of elements in the array
	* Thread safe, but you know, someone might have added more elements before this even returns
	* @return	the number of elements in the array
	**/
	FORCEINLINE int32 Num() const
	{
		return NumElements;
	}
	/**
	* Return if this index is valid
	* Thread safe, if it is valid now, it is valid forever. Other threads might be adding during this call.
	* @param	Index	Index to test
	* @return	true, if this is a valid
	**/
	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index < Num() && Index >= 0;
	}
	/**
	* Return a reference to an element
	* @param	Index	Index to return
	* @return	a reference to the pointer to the element
	* Thread safe, if it is valid now, it is valid forever. This might return nullptr, but by then, some other thread might have made it non-nullptr.
	**/
	FORCEINLINE FUObjectItem const& operator[](int32 Index) const
	{
		FUObjectItem const* ItemPtr = GetObjectPtr(Index);
		check(ItemPtr);
		return *ItemPtr;
	}

	FORCEINLINE FUObjectItem& operator[](int32 Index)
	{
		FUObjectItem* ItemPtr = GetObjectPtr(Index);
		check(ItemPtr);
		return *ItemPtr;
	}
};

class FChunkedFixedUObjectArray
{
public:
	enum
	{
		NumElementsPerChunk = 0x10000,
	};

	/** Master table to chunks of pointers **/
	FUObjectItem** Objects;
	/** If requested, a contiguous memory where all objects are allocated **/
	FUObjectItem* PreAllocatedObjects;
	/** Maximum number of elements **/
	const int32 MaxElements;
	/** Number of elements we currently have **/
	const int32 NumElements;
	/** Maximum number of chunks **/
	const int32 MaxChunks;
	/** Number of chunks we currently have **/
	const int32 NumChunks;

public:
	/**
	* Return the number of elements in the array
	* Thread safe, but you know, someone might have added more elements before this even returns
	* @return	the number of elements in the array
	**/
	FORCEINLINE int32 Num() const
	{
		return NumElements;
	}

	/**
	* Return the number max capacity of the array
	* Thread safe, but you know, someone might have added more elements before this even returns
	* @return	the maximum number of elements in the array
	**/
	FORCEINLINE int32 Capacity() const
	{
		return MaxElements;
	}

	/**
	* Return if this index is valid
	* Thread safe, if it is valid now, it is valid forever. Other threads might be adding during this call.
	* @param	Index	Index to test
	* @return	true, if this is a valid
	**/
	FORCEINLINE bool IsValidIndex(int32 Index) const
	{
		return Index < Num() && Index >= 0;
	}

	/**
	* Return a pointer to the pointer to a given element
	* @param Index The Index of an element we want to retrieve the pointer-to-pointer for
	**/
	FUObjectItem const* GetObjectPtr(int32 Index) const
	{
		const int32 ChunkIndex = Index / NumElementsPerChunk;
		const int32 WithinChunkIndex = Index % NumElementsPerChunk;
		FUObjectItem* Chunk = Objects[ChunkIndex];
		return Chunk + WithinChunkIndex;
	}
	FUObjectItem* GetObjectPtr(int32 Index)
	{
		if (Index < 0 || Index > Capacity())
			return nullptr;

		const int32 ChunkIndex = Index / NumElementsPerChunk;
		const int32 WithinChunkIndex = Index % NumElementsPerChunk;

		return Objects[ChunkIndex] + WithinChunkIndex;
	}

	/**
	* Return a reference to an element
	* @param	Index	Index to return
	* @return	a reference to the pointer to the element
	* Thread safe, if it is valid now, it is valid forever. This might return nullptr, but by then, some other thread might have made it non-nullptr.
	**/
	FORCEINLINE FUObjectItem const& operator[](int32 Index) const
	{
		FUObjectItem const* ItemPtr = GetObjectPtr(Index);
		return *ItemPtr;
	}
	FORCEINLINE FUObjectItem& operator[](int32 Index)
	{
		FUObjectItem* ItemPtr = GetObjectPtr(Index);
		return *ItemPtr;
	}

	/**
	* Return a naked pointer to the fundamental data structure for debug visualizers.
	**/
	FUObjectItem*** GetRootBlockForDebuggerVisualizers()
	{
		return nullptr;
	}

	int64 GetAllocatedSize() const
	{
		return MaxChunks * sizeof(FUObjectItem*) + NumChunks * NumElementsPerChunk * sizeof(FUObjectItem);
	}
};

class FUObjectArray
{
public:
	static inline FUObjectItem* IndexToObject(int32 Index)
	{
		if (!Finder::FindGUObjectArray()) return 0;

		if (Version::Engine_Version >= 4.21)
		{
			FChunkedFixedUObjectArray* ChunkedArray = (FChunkedFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return ChunkedArray->GetObjectPtr(Index);
		}
		else
		{
			FFixedUObjectArray* FixedArray = (FFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return FixedArray->GetObjectPtr(Index);
		}
	}

	static inline int32 Num() {
		if (!Finder::FindGUObjectArray()) return 0;

		if (Version::Engine_Version >= 4.21)
		{
			FChunkedFixedUObjectArray* ChunkedArray = (FChunkedFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return ChunkedArray->Num();
		}
		else
		{
			FFixedUObjectArray* FixedArray = (FFixedUObjectArray*)(ImageBase + Finder::FindGUObjectArray());
			return FixedArray->Num();
		}
	}

	static UObject* FindObject(const std::string& FullName, bool bStrictCheck = true, EClassCastFlags Flags = EClassCastFlags::RF_NoFlags);
	static UObject* FindObjectWithClass(const std::string& FullName, bool bStrictCheck = true, UClass* Class = nullptr);

	static UObject* FindObjectFast(const std::string& Name);

	static UObject* DefaultObjImpl(const UClass* TargetClass);

	static TArray<UObject*> GetObjectsOfClass(UClass* TargetClass, std::string MustIncludeString = "");

	static TArray<UObject*> FindObjects(const std::string& SearchString);
};

inline FUObjectArray GUObjectArray;