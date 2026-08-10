#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeCompatibleBytes.h"
#include "Engine/Source/Runtime/Core/Public/Templates/TypeHash.h"
#include "Engine/Source/Runtime/Core/Public/Containers/ContainerAllocationPolicies.h"
#include "SparseArray.h"
#include "BitArray.h"

template <typename SetType>
class TSetElement
{
private:
    template <typename SetDataType>
    friend class TSet;

public:
    SetType Value;
    int32_t HashNextId;
    int32_t HashIndex;

public:
    TSetElement() : Value(), HashNextId(-1), HashIndex(-1) {}

    TSetElement(const SetType& InValue)
        : Value(InValue)
        , HashNextId(-1)
        , HashIndex(-1)
    {
    }
};

template <typename SetType>
using SetElement = TSetElement<SetType>;

template <typename SetElementType>
class TSet
{
private:
    static constexpr uint32_t ElementAlign = alignof(SetElementType);
    static constexpr uint32_t ElementSize = sizeof(SetElementType);

private:
    using SetDataType = TSetElement<SetElementType>;
    using HashType = TInlineAllocator<1>::ForElementType<int32_t>;

public:
    TSparseArray<SetDataType> Elements;
    HashType Hash;
    int32_t HashSize;

public:
    TSet()
        : HashSize(0)
    {
    }

private:
    inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

    inline int32_t* GetHashBucket(int32 HashIndex)
    {
        return Hash.GetAllocation() + (HashIndex & (HashSize - 1));
    }

    void UnlinkFromHashChain(int32 ElementIndex)
    {
        if (HashSize <= 0)
            return;

        SetDataType& ElementBeingRemoved = Elements[ElementIndex];

        int32_t* NextElementIdPtr = GetHashBucket(ElementBeingRemoved.HashIndex);
        for (int32 Guard = Elements.NumAllocated(); Guard >= 0 && *NextElementIdPtr != -1; --Guard)
        {
            if (*NextElementIdPtr == ElementIndex)
            {
                *NextElementIdPtr = ElementBeingRemoved.HashNextId;
                break;
            }

            if (!Elements.IsAllocated(*NextElementIdPtr))
                break;

            NextElementIdPtr = &Elements[*NextElementIdPtr].HashNextId;
        }

        ElementBeingRemoved.HashNextId = -1;
        ElementBeingRemoved.HashIndex = -1;
    }

    void LinkElement(int32 ElementIndex, uint32 KeyHash)
    {
        SetDataType& Element = Elements[ElementIndex];

        Element.HashIndex = (int32)(KeyHash & (uint32)(HashSize - 1));

        int32_t* Bucket = Hash.GetAllocation() + Element.HashIndex;
        Element.HashNextId = *Bucket;
        *Bucket = ElementIndex;
    }

    static uint32 GetNumberOfHashBuckets(uint32 NumHashedElements)
    {
        const uint32 ElementsPerBucket = 2;
        const uint32 BaseNumberOfBuckets = 8;
        const uint32 MinNumberOfHashedElements = 4;

        if (NumHashedElements >= MinNumberOfHashedElements)
        {
            return RoundUpToPowerOfTwo(NumHashedElements / ElementsPerBucket + BaseNumberOfBuckets);
        }

        return 1;
    }

    template<typename KeyHashFuncType>
    void Rehash(KeyHashFuncType GetKeyHash)
    {
        Hash.ResizeAllocation(0, HashSize, sizeof(int32_t));

        int32_t* Buckets = Hash.GetAllocation();
        for (int32 i = 0; i < HashSize; ++i)
        {
            Buckets[i] = -1;
        }

        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            LinkElement(It.GetIndex(), GetKeyHash(Elements[It.GetIndex()].Value));
        }
    }

    template<typename KeyHashFuncType>
    bool ConditionalRehash(int32 NumHashedElements, KeyHashFuncType GetKeyHash)
    {
        const uint32 DesiredHashSize = GetNumberOfHashBuckets((uint32)NumHashedElements);

        if (NumHashedElements > 0 && (HashSize <= 0 || (uint32)HashSize < DesiredHashSize))
        {
            HashSize = (int32)DesiredHashSize;
            Rehash(GetKeyHash);
            return true;
        }

        return false;
    }

private:
    static inline bool bHashUnreliable = false;

    template <typename PredicateType>
    int32 FindIdLinearBy(PredicateType Predicate) const
    {
        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            const int32 Index = It.GetIndex();
            if (Predicate(Elements[Index].Value))
                return Index;
        }

        return -1;
    }

public:
    template <typename PredicateType>
    int32 FindIdByHashBy(uint32 KeyHash, PredicateType Predicate) const
    {
        const int32 NumAllocatedElements = Elements.NumAllocated();

        if (bHashUnreliable || HashSize <= 0 || (HashSize & (HashSize - 1)) != 0 || !Hash.GetAllocation())
            return FindIdLinearBy(Predicate);

        int32 Guard = NumAllocatedElements + 1;

        for (int32 Index = Hash.GetAllocation()[KeyHash & (uint32)(HashSize - 1)];
             Index != -1 && Guard-- > 0;
             Index = Elements[Index].HashNextId)
        {
            if (Index < 0 || Index >= NumAllocatedElements)
                break;

            if (!Elements.IsAllocated(Index))
                break;

            if (Predicate(Elements[Index].Value))
                return Index;
        }

        const int32 LinearIndex = FindIdLinearBy(Predicate);
        if (LinearIndex != -1)
        {
            bHashUnreliable = true;
            Log("TSet: a hashed lookup missed an element that a linear scan found. GetTypeHash disagrees "
                "with the hash chains this container was built with - falling back to linear lookups for "
                "this element type.");
        }

        return LinearIndex;
    }

public:
    inline int32 NumAllocated() const { return Elements.NumAllocated(); }

    inline int32 Num() const { return Elements.Num(); }
    inline int32 Max() const { return Elements.Max(); }

    inline bool IsValidIndex(int32 Index) const { return Elements.IsValidIndex(Index); }

    inline bool IsValid() const { return Elements.IsValid(); }

    const FBitArray& GetAllocationFlags() const { return Elements.GetAllocationFlags(); }

public:
    inline       SetElementType& operator[] (int32 Index) { return Elements[Index].Value; }
    inline const SetElementType& operator[] (int32 Index) const { return Elements[Index].Value; }

    inline bool operator==(const TSet<SetElementType>& Other) const { return Elements == Other.Elements; }
    inline bool operator!=(const TSet<SetElementType>& Other) const { return Elements != Other.Elements; }

public:
    int32 Add(const SetElementType& Item)
    {
        return AddWithKeyHashFunc(Item, [](const SetElementType& Value) { return GetTypeHash(Value); });
    }

    template<typename KeyHashFuncType>
    int32 AddWithKeyHashFunc(const SetElementType& Item, KeyHashFuncType GetKeyHash)
    {
        const int32 Index = Elements.AddUninitializedIndex();

        SetDataType& Element = Elements[Index];
        std::memcpy((void*)&Element.Value, (const void*)&Item, sizeof(SetElementType));
        Element.HashNextId = -1;
        Element.HashIndex = -1;

        if (!ConditionalRehash(Elements.Num(), GetKeyHash))
        {
            LinkElement(Index, GetKeyHash(Element.Value));
        }

        return Index;
    }

    const SetElementType* Find(const SetElementType& Item) const
    {
        const int32 Index = FindId(Item);
        return Index != -1 ? &Elements[Index].Value : nullptr;
    }

    SetElementType* Find(const SetElementType& Item)
    {
        const int32 Index = FindId(Item);
        return Index != -1 ? &Elements[Index].Value : nullptr;
    }

    int32 FindId(const SetElementType& Item) const
    {
        return FindIdByHashBy(GetTypeHash(Item),
            [&Item](const SetElementType& Value) { return Value == Item; });
    }

    template <typename ComparisonType>
    bool Contains(ComparisonType Item) const
    {
        return FindIdByHashBy(GetTypeHash(Item),
            [&Item](const SetElementType& Value) { return Value == Item; }) != -1;
    }

    bool Remove(const SetElementType& Item)
    {
        const int32 Index = FindId(Item);
        if (Index != -1)
        {
            RemoveAt(Index);
            return true;
        }

        return false;
    }

    void RemoveAt(int32 Index)
    {
        if (Elements.IsValidIndex(Index))
        {
            UnlinkFromHashChain(Index);
            Elements.RemoveAt(Index);
        }
    }

    void Reset()
    {
        Elements.Reset();

        int32_t* Buckets = Hash.GetAllocation();
        for (int32 i = 0; i < HashSize; ++i)
        {
            Buckets[i] = -1;
        }
    }

    void Empty(int32 ExpectedNumElements = 0)
    {
        Reset();
    }

    template <typename... ArgsType>
    int32 Emplace(ArgsType&&... Args)
    {
        SetElementType Temp(std::forward<ArgsType>(Args)...);
        return Add(Temp);
    }

    TArray<SetElementType> Array() const
    {
        TArray<SetElementType> Result;
        Result.Reserve(Num());

        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            Result.Add(Elements[It.GetIndex()].Value);
        }

        return Result;
    }

    TSet<SetElementType> Union(const TSet<SetElementType>& OtherSet) const
    {
        TSet<SetElementType> Result;

        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            Result.Add(Elements[It.GetIndex()].Value);
        }
        for (FBitArray::FSetBitIterator It(OtherSet.Elements.GetAllocationFlags()); It; ++It)
        {
            const SetElementType& Value = OtherSet.Elements[It.GetIndex()].Value;
            if (!Result.Contains(Value))
            {
                Result.Add(Value);
            }
        }

        return Result;
    }

    TSet<SetElementType> Intersect(const TSet<SetElementType>& OtherSet) const
    {
        TSet<SetElementType> Result;

        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            const SetElementType& Value = Elements[It.GetIndex()].Value;
            if (OtherSet.Contains(Value))
            {
                Result.Add(Value);
            }
        }

        return Result;
    }

    TSet<SetElementType> Difference(const TSet<SetElementType>& OtherSet) const
    {
        TSet<SetElementType> Result;

        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            const SetElementType& Value = Elements[It.GetIndex()].Value;
            if (!OtherSet.Contains(Value))
            {
                Result.Add(Value);
            }
        }

        return Result;
    }

    bool Includes(const TSet<SetElementType>& OtherSet) const
    {
        for (FBitArray::FSetBitIterator It(OtherSet.Elements.GetAllocationFlags()); It; ++It)
        {
            if (!Contains(OtherSet.Elements[It.GetIndex()].Value))
            {
                return false;
            }
        }

        return true;
    }

public:
    class TIterator
    {
    private:
        TSet& Set;
        FBitArray::FSetBitIterator BitIt;

    public:
        TIterator(const TSet& InSet, int32 StartIndex = 0)
            : Set(const_cast<TSet&>(InSet))
            , BitIt(InSet.GetAllocationFlags(), StartIndex)
        {
        }

    public:
        inline int32 GetIndex() { return BitIt.GetIndex(); }

        inline TIterator& operator++() { ++BitIt; return *this; }

        inline explicit operator bool() const { return (bool)BitIt; }

        inline SetElementType& operator*() { return Set.Elements[BitIt.GetIndex()].Value; }
        inline SetElementType* operator->() { return &Set.Elements[BitIt.GetIndex()].Value; }

        inline bool operator==(const TIterator& Other) const { return BitIt == Other.BitIt; }
        inline bool operator!=(const TIterator& Other) const { return BitIt != Other.BitIt; }
    };

    inline TIterator begin() { return TIterator(*this, 0); }
    inline TIterator end() { return TIterator(*this, NumAllocated()); }
    inline TIterator begin() const { return TIterator(*this, 0); }
    inline TIterator end() const { return TIterator(*this, NumAllocated()); }

    inline TIterator CreateIterator() { return TIterator(*this, 0); }
    inline TIterator CreateConstIterator() const { return TIterator(*this, 0); }
};

static_assert(sizeof(TSetElement<int32>) == 0xC, "TSetElement layout broke: Value/HashNextId/HashIndex expected");
static_assert(sizeof(TSet<int32>) == 0x50, "TSet layout broke: UE 4.0-5.x expects sparse array + inline-1 hash + HashSize = 0x50 on x64");
static_assert(offsetof(TSet<int32>, Hash) == 0x38, "TSet::Hash must sit at 0x38 to match the engine");
static_assert(offsetof(TSet<int32>, HashSize) == 0x48, "TSet::HashSize must sit at 0x48 to match the engine");
