#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"

template<uint32 NumInlineElements>
class TInlineAllocator
{
public:
    template<typename ElementType>
    class ForElementType
    {
    private:
        static constexpr int32 ElementSize = sizeof(ElementType);
        static constexpr int32 ElementAlign = alignof(ElementType);

        static constexpr int32 InlineDataSizeBytes = NumInlineElements * ElementSize;

    private:
        TAlignedBytes<ElementSize, ElementAlign> InlineData[NumInlineElements];
        ElementType* SecondaryData;

    public:
        ForElementType()
            : InlineData{ 0x0 }, SecondaryData(nullptr)
        {
        }

        ForElementType(ForElementType&&) = default;
        ForElementType(const ForElementType&) = default;

    public:
        ForElementType& operator=(ForElementType&&) = default;
        ForElementType& operator=(const ForElementType&) = default;

    public:
        inline const ElementType* GetAllocation() const { return SecondaryData ? SecondaryData : reinterpret_cast<const ElementType*>(&InlineData); }
        inline ElementType* GetAllocation() { return SecondaryData ? SecondaryData : reinterpret_cast<ElementType*>(&InlineData); }

        inline uint32 GetNumInlineBytes() const { return NumInlineElements; }
    };
};

class FBitArray
{
protected:
    static constexpr int32 NumBitsPerDWORD = 32;
    static constexpr int32 NumBitsPerDWORDLogTwo = 5;

private:
    TInlineAllocator<4>::ForElementType<int32> Data;
    int32 NumBits;
    int32 MaxBits;

public:
    FBitArray()
        : NumBits(0), MaxBits(Data.GetNumInlineBytes()* NumBitsPerDWORD)
    {
    }

    FBitArray(const FBitArray&) = default;

    FBitArray(FBitArray&&) = default;

public:
    FBitArray& operator=(FBitArray&&) = default;

    FBitArray& operator=(const FBitArray& Other) = default;

public:
    inline int32 Num() const { return NumBits; }
    inline int32 Max() const { return MaxBits; }

    inline uint32* GetData() { return reinterpret_cast<uint32*>(Data.GetAllocation()); }
    inline const uint32* GetData() const { return reinterpret_cast<const uint32*>(Data.GetAllocation()); }

    inline bool IsValidIndex(int32 Index) const { return Index >= 0 && Index < NumBits; }

    inline bool IsValid() const { return GetData() && NumBits > 0; }

public:
    inline bool operator[](int32 Index) const { return GetData()[Index / NumBitsPerDWORD] & (1 << (Index & (NumBitsPerDWORD - 1))); }

    inline bool operator==(const FBitArray& Other) const { return NumBits == Other.NumBits && GetData() == Other.GetData(); }
    inline bool operator!=(const FBitArray& Other) const { return NumBits != Other.NumBits || GetData() != Other.GetData(); }

    inline void Set(const int32 Index, const bool Value, bool bIsSettingAllZero = false)
    {
        const int32 DWORDIndex = (Index >> ((int32)5));
        const int32 Mask = (1 << (Index & (((int32)32) - 1)));

        if (!bIsSettingAllZero)
            NumBits = Index >= NumBits ? Index < MaxBits ? Index + 1 : NumBits : NumBits;

        Value ? GetData()[DWORDIndex] |= Mask : GetData()[DWORDIndex] &= ~Mask;
    }

public:
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
    public:
        inline uint32 FloorLog2()
        {
            uint32 pos = 0;
            if (Mask >= 1 << 16) { Mask >>= 16; pos += 16; }
            if (Mask >= 1 << 8) { Mask >>= 8; pos += 8; }
            if (Mask >= 1 << 4) { Mask >>= 4; pos += 4; }
            if (Mask >= 1 << 2) { Mask >>= 2; pos += 2; }
            if (Mask >= 1 << 1) { pos += 1; }
            return pos;
        }

        inline uint32 CountLeadingZeros()
        {
            if (Mask == 0)
                return 32;

            return 31 - FloorLog2();
        }
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

            CurrentBitIndex = BaseBitIndex + NumBitsPerDWORD - 1 - CountLeadingZeros();

            if (CurrentBitIndex > ArrayNum)
                CurrentBitIndex = ArrayNum;
        }
    };

    inline FSetBitIterator begin() { return FSetBitIterator(*this, 0); }
    inline FSetBitIterator end() { return FSetBitIterator(*this, Num()); }
};