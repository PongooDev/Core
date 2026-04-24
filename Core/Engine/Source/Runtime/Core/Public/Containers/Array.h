#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/HAL/UnrealMemory.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

template<typename ArrayElementType>
class TArray
{
protected:
	static constexpr uint64 ElementSize = sizeof(ArrayElementType);

public:
	ArrayElementType* Data;
	int32 ArrayNum;
	int32 ArrayMax;

public:
	TArray()
		: Data(nullptr), ArrayNum(0), ArrayMax(0)
	{
	}

	TArray(const TArray&) = default;

	TArray(TArray&&) = default;

public:
	TArray& operator=(TArray&&) = default;
	TArray& operator=(const TArray&) = default;

public:
	inline int32 GetSlack() const { return ArrayMax - ArrayNum; }

	inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

	inline       ArrayElementType& GetUnsafe(int32 Index) { return Data[Index]; }
	inline const ArrayElementType& GetUnsafe(int32 Index) const { return Data[Index]; }

	inline ArrayElementType& GetWithSize(int32 Index, int32 Size = ElementSize)
	{
		return *(ArrayElementType*)((uint8*)Data + Index * Size);
	}
	inline const ArrayElementType& GetWithSize(int32 Index, int32 Size = ElementSize) const
	{
		return *(ArrayElementType*)((uint8*)Data + Index * Size);
	}

public:
	void CheckInvariants() const
	{
		checkSlow((ArrayNum >= 0) & (ArrayMax >= ArrayNum));
	}

	void DestructItems(ArrayElementType* Items, int32 Count)
	{
		if constexpr (!std::is_trivially_destructible_v<ArrayElementType>)
		{
			for (int32 i = 0; i < Count; ++i)
			{
				Items[i].~ArrayElementType();
			}
		}
	}

	void ResizeShrink(int32 Size = ElementSize)
	{
		if (ArrayNum != ArrayMax)
		{
			ArrayMax = ArrayNum;
			check(ArrayMax >= ArrayNum);
			ResizeTo(ArrayMax, Size);
		}
	}

	void ResizeTo(int32 NewMax, int32 Size = ElementSize)
	{
		if (NewMax != ArrayMax)
		{
			ArrayMax = NewMax;
			void* NewData = FMemory::Realloc(Data, ArrayMax * Size);
			if (NewData)
			{
				Data = (ArrayElementType*)NewData;
			}
		}
	}

	inline void Reserve(int32 Number, int32 Size = ElementSize)
	{
		if (Number > ArrayMax)
		{
			ResizeTo(Number, Size);
		}
	}

	int32 Add(const ArrayElementType& Item, int32 Size = ElementSize) {
		Reserve(ArrayNum + 1, ElementSize);
		std::memcpy(PBYTE(Data) + (ArrayNum * Size), (const PBYTE)&Item, Size);
		return ArrayNum++;
	}

	void RemoveAtImpl(int32 Index, int32 Count, bool bAllowShrinking, int32 Size = ElementSize)
	{
		if (!IsValidIndex(Index))
			return;

		if (Count)
		{
			ArrayNum--;

			for (int i = Index; i < ArrayNum; i++)
			{
				Data[i] = Data[i + 1];
			}
		}
	}

	void RemoveAt(int32 Index, int32 Size = ElementSize)
	{
		RemoveAtImpl(Index, 1, true, Size);
	}

	template <typename CountType>
	void RemoveAt(int32 Index, CountType Count, bool bAllowShrinking = true, int32 Size = ElementSize)
	{
		RemoveAtImpl(Index, Count, bAllowShrinking, Size);
	}

	void Reset(int32 NewSize = 0, int32 Size = ElementSize)
	{
		if constexpr (!std::is_trivially_destructible_v<ArrayElementType>)
		{
			for (int32 i = 0; i < ArrayNum; ++i)
			{
				Data[i].~ArrayElementType();
			}
		}

		ArrayNum = 0;

		if (NewSize > 0 && NewSize > ArrayMax)
		{
			Reserve(NewSize - ArrayNum, Size);
		}
	}

	void Empty(int32 Slack = 0, int32 Size = ElementSize)
	{
		DestructItems(GetData(), ArrayNum);

		ArrayNum = 0;

		if (ArrayMax != Slack)
		{
			ResizeTo(Slack, Size);
		}
	}

	template <typename ComparisonType>
	bool Contains(const ComparisonType& Item) const
	{
		const ArrayElementType* DataPtr = GetData();
		const ArrayElementType* DataEnd = DataPtr + ArrayNum;

		for (; DataPtr != DataEnd; ++DataPtr)
		{
			if (*DataPtr == Item)
			{
				return true;
			}
		}
		return false;
	}
public:
	inline int32 Num() const { return ArrayNum; }
	inline int32 Max() const { return ArrayMax; }

	inline bool IsValidIndex(int32 Index) const { return Data && Index >= 0 && Index < ArrayNum; }

	inline bool IsValid() const { return Data && ArrayNum > 0 && ArrayMax >= ArrayNum; }

	FORCEINLINE ArrayElementType* GetData()
	{
		return (ArrayElementType*)Data;
	}

	FORCEINLINE const ArrayElementType* GetData() const
	{
		return (const ArrayElementType*)Data;
	}

public:
	inline       ArrayElementType& operator[](int32 Index) { VerifyIndex(Index); return Data[Index]; }
	inline const ArrayElementType& operator[](int32 Index) const { VerifyIndex(Index); return Data[Index]; }

	inline bool operator==(const TArray<ArrayElementType>& Other) const { return Data == Other.Data; }
	inline bool operator!=(const TArray<ArrayElementType>& Other) const { return Data != Other.Data; }

	inline explicit operator bool() const { return IsValid(); };

public:
	ArrayElementType* begin()
	{
		return Data;
	}

	ArrayElementType* end()
	{
		return Data + ArrayNum;
	}

	const ArrayElementType* begin() const
	{
		return Data;
	}

	const ArrayElementType* end() const
	{
		return Data + ArrayNum;
	}
};
