#pragma once
#include "pch.h"

#include "BitArray.h"

template <typename KeyType, typename ValueType>
class TMap;

template <typename SetElementType>
class TSet;

template <typename SparseArrayElementType>
class TSparseArray;

namespace ContainerHelperFunctions {
	inline uint32 FloorLog2(uint32 Value)
	{
		uint32 pos = 0;
		if (Value >= 1 << 16) { Value >>= 16; pos += 16; }
		if (Value >= 1 << 8) { Value >>= 8; pos += 8; }
		if (Value >= 1 << 4) { Value >>= 4; pos += 4; }
		if (Value >= 1 << 2) { Value >>= 2; pos += 2; }
		if (Value >= 1 << 1) { pos += 1; }
		return pos;
	}

	inline uint32 CountLeadingZeros(uint32 Value)
	{
		if (Value == 0)
			return 32;

		return 31 - FloorLog2(Value);
	}
}

namespace ContainerIterators {
	class FSetBitIterator;

	template<typename ArrayType>
	class TArrayIterator;

	template<class ContainerType>
	class TContainerIterator;

	template<typename SparseArrayElementType>
	using TSparseArrayIterator = TContainerIterator<TSparseArray<SparseArrayElementType>>;

	template<typename SetElementType>
	using TSetIterator = TContainerIterator<TSet<SetElementType>>;

	template<typename KeyElementType, typename ValueElementType>
	using TMapIterator = TContainerIterator<TMap<KeyElementType, ValueElementType>>;

	class FRelativeBitReference
	{
	protected:
		static constexpr int32 NumBitsPerDWORD = 32;
		static constexpr int32 NumBitsPerDWORDLogTwo = 5;

	public:
		inline explicit FRelativeBitReference(int32 BitIndex)
			: WordIndex(BitIndex >> NumBitsPerDWORDLogTwo)
			, Mask(1 << (BitIndex & (NumBitsPerDWORD - 1)))
		{
		}

		int32  WordIndex;
		uint32 Mask;
	};

	class FSetBitIterator : public FRelativeBitReference
	{
	private:
		const FBitArray& Array;

		uint32 UnvisitedBitMask;
		int32 CurrentBitIndex;
		int32 BaseBitIndex;

	public:
		explicit FSetBitIterator(const FBitArray& InArray, int32 StartIndex = 0)
			: FRelativeBitReference(StartIndex)
			, Array(InArray)
			, UnvisitedBitMask((~0U) << (StartIndex & (NumBitsPerDWORD - 1)))
			, CurrentBitIndex(StartIndex)
			, BaseBitIndex(StartIndex & ~(NumBitsPerDWORD - 1))
		{
			if (StartIndex != Array.Num())
				FindFirstSetBit();
		}

	public:
		inline FSetBitIterator& operator++()
		{
			UnvisitedBitMask &= ~this->Mask;

			FindFirstSetBit();

			return *this;
		}

		inline explicit operator bool() const { return CurrentBitIndex < Array.Num(); }

		inline bool operator==(const FSetBitIterator& Rhs) const { return CurrentBitIndex == Rhs.CurrentBitIndex && &Array == &Rhs.Array; }
		inline bool operator!=(const FSetBitIterator& Rhs) const { return CurrentBitIndex != Rhs.CurrentBitIndex || &Array != &Rhs.Array; }

	public:
		inline int32 GetIndex() { return CurrentBitIndex; }

		void FindFirstSetBit()
		{
			const uint32* ArrayData = Array.GetData();
			const int32   ArrayNum = Array.Num();
			const int32   LastWordIndex = (ArrayNum - 1) / NumBitsPerDWORD;

			uint32 RemainingBitMask = ArrayData[this->WordIndex] & UnvisitedBitMask;
			while (!RemainingBitMask)
			{
				++this->WordIndex;
				BaseBitIndex += NumBitsPerDWORD;
				if (this->WordIndex > LastWordIndex)
				{
					CurrentBitIndex = ArrayNum;
					return;
				}

				RemainingBitMask = ArrayData[this->WordIndex];
				UnvisitedBitMask = ~0;
			}

			const uint32 NewRemainingBitMask = RemainingBitMask & (RemainingBitMask - 1);

			this->Mask = NewRemainingBitMask ^ RemainingBitMask;

			CurrentBitIndex = BaseBitIndex + NumBitsPerDWORD - 1 - ContainerHelperFunctions::CountLeadingZeros(this->Mask);

			if (CurrentBitIndex > ArrayNum)
				CurrentBitIndex = ArrayNum;
		}
	};

	template<class ContainerType>
	class TContainerIterator
	{
	private:
		ContainerType& IteratedContainer;
		FSetBitIterator BitIterator;

	public:
		TContainerIterator(const ContainerType& Container, const FBitArray& BitArray, int32 StartIndex = 0x0)
			: IteratedContainer(const_cast<ContainerType&>(Container)), BitIterator(BitArray, StartIndex)
		{
		}

	public:
		inline int32 GetIndex() { return BitIterator.GetIndex(); }

		inline int32 IsValid() { return IteratedContainer.IsValidIndex(GetIndex()); }

	public:
		inline TContainerIterator& operator++() { ++BitIterator; return *this; }
		inline TContainerIterator& operator--() { --BitIterator; return *this; }

		inline       auto& operator*() { return IteratedContainer[GetIndex()]; }
		inline const auto& operator*() const { return IteratedContainer[GetIndex()]; }

		inline       auto* operator->() { return &IteratedContainer[GetIndex()]; }
		inline const auto* operator->() const { return &IteratedContainer[GetIndex()]; }

		inline bool operator==(const TContainerIterator& Other) const { return &IteratedContainer == &Other.IteratedContainer && BitIterator == Other.BitIterator; }
		inline bool operator!=(const TContainerIterator& Other) const { return &IteratedContainer != &Other.IteratedContainer || BitIterator != Other.BitIterator; }
	};
}