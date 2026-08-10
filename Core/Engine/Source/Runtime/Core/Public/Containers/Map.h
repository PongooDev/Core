#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Templates/Tuple.h"
#include "Set.h"

template<typename KeyElementType, typename ValueElementType>
class TMap
{
public:
	using ElementType = TPair<KeyElementType, ValueElementType>;

public:
	TSet<ElementType> Elements;

private:
	inline void VerifyIndex(int32 Index) const { if (!IsValidIndex(Index)) throw std::out_of_range("Index was out of range!"); }

protected:
	static uint32 GetPairKeyHash(const ElementType& Pair)
	{
		return GetTypeHash(Pair.Key());
	}

public:
	inline int32 NumAllocated() const { return Elements.NumAllocated(); }

	inline int32 Num() const { return Elements.Num(); }
	inline int32 Max() const { return Elements.Max(); }

	inline bool IsValidIndex(int32 Index) const { return Elements.IsValidIndex(Index); }

	inline bool IsValid() const { return Elements.IsValid(); }

public:
	const FBitArray& GetAllocationFlags() const { return Elements.GetAllocationFlags(); }
public:
    ValueElementType* Find(const KeyElementType& Key)
    {
        const int32 Index = Elements.FindIdByHashBy(GetTypeHash(Key),
            [&Key](const ElementType& Pair) { return Pair.Key() == Key; });

        return Index != -1 ? &Elements[Index].Value() : nullptr;
    }

    const ValueElementType* Find(const KeyElementType& Key) const
    {
        return const_cast<TMap*>(this)->Find(Key);
    }

    ValueElementType FindRef(const KeyElementType& Key) const
    {
        const ValueElementType* Found = Find(Key);
        if (!Found)
        {
            return ValueElementType{};
        }
        return *Found;
    }

    bool Contains(const KeyElementType& Key) const
    {
        return Find(Key) != nullptr;
    }

    template <class PT>
    ValueElementType* Search(PT Predicate) {
        for (auto& [k, v] : *this) {
            if (Predicate(k, v)) return &v;
        }
        return nullptr;
    }

    ValueElementType& Add(const KeyElementType& Key, const ValueElementType& Value)
    {
        if (ValueElementType* Existing = Find(Key))
        {
            std::memcpy((void*)Existing, (const void*)&Value, sizeof(ValueElementType));
            return *Existing;
        }

        const int32 Index = Elements.AddWithKeyHashFunc(ElementType(Key, Value), &TMap::GetPairKeyHash);
        return Elements[Index].Value();
    }

    ValueElementType& FindOrAdd(const KeyElementType& Key)
    {
        if (ValueElementType* Existing = Find(Key))
        {
            return *Existing;
        }

        return Add(Key, ValueElementType{});
    }

    bool Remove(const KeyElementType& Key)
    {
        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            int32 Index = It.GetIndex();
            if (Elements[Index].Key() == Key)
            {
                Elements.RemoveAt(Index);
                return true;
            }
        }

        return false;
    }

    template <typename... ArgsType>
    ValueElementType& Emplace(const KeyElementType& Key, ArgsType&&... Args)
    {
        ValueElementType Temp(std::forward<ArgsType>(Args)...);
        return Add(Key, Temp);
    }

    ValueElementType& FindChecked(const KeyElementType& Key)
    {
        ValueElementType* Found = Find(Key);
        if (!Found)
        {
            throw std::out_of_range("TMap::FindChecked: key not found!");
        }
        return *Found;
    }

    const ValueElementType& FindChecked(const KeyElementType& Key) const
    {
        return const_cast<TMap*>(this)->FindChecked(Key);
    }

    const KeyElementType* FindKey(const ValueElementType& Value) const
    {
        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            const ElementType& Pair = Elements[It.GetIndex()];
            if (Pair.Value() == Value)
            {
                return &Pair.Key();
            }
        }

        return nullptr;
    }

    void GenerateKeyArray(TArray<KeyElementType>& OutArray) const
    {
        OutArray.Empty(Num());
        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            OutArray.Add(Elements[It.GetIndex()].Key());
        }
    }

    void GenerateValueArray(TArray<ValueElementType>& OutArray) const
    {
        OutArray.Empty(Num());
        for (FBitArray::FSetBitIterator It(Elements.GetAllocationFlags()); It; ++It)
        {
            OutArray.Add(Elements[It.GetIndex()].Value());
        }
    }

    void Append(const TMap<KeyElementType, ValueElementType>& OtherMap)
    {
        for (FBitArray::FSetBitIterator It(OtherMap.Elements.GetAllocationFlags()); It; ++It)
        {
            const ElementType& Pair = OtherMap.Elements[It.GetIndex()];
            Add(Pair.Key(), Pair.Value());
        }
    }

    void Reset()
    {
        Elements.Reset();
    }

    void Empty(int32 ExpectedNumElements = 0)
    {
        Elements.Empty(ExpectedNumElements);
    }

public:
	inline       ElementType& operator[] (int32 Index) { return Elements[Index]; }
	inline const ElementType& operator[] (int32 Index) const { return Elements[Index]; }

	inline bool operator==(const TMap<KeyElementType, ValueElementType>& Other) const { return Elements == Other.Elements; }
	inline bool operator!=(const TMap<KeyElementType, ValueElementType>& Other) const { return Elements != Other.Elements; }

public:
	class TIterator
	{
	private:
		TMap& Map;
		FBitArray::FSetBitIterator BitIt;

	public:
		TIterator(const TMap& InMap, int32 StartIndex = 0)
			: Map(const_cast<TMap&>(InMap))
			, BitIt(InMap.GetAllocationFlags(), StartIndex)
		{
		}

	public:
		inline int32 GetIndex() { return BitIt.GetIndex(); }

		inline TIterator& operator++() { ++BitIt; return *this; }

		inline explicit operator bool() const { return (bool)BitIt; }

		inline ElementType& operator*() { return Map.Elements[BitIt.GetIndex()]; }
		inline ElementType* operator->() { return &Map.Elements[BitIt.GetIndex()]; }

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

template<typename KeyElementType, typename ValueElementType>
class TMultiMap : public TMap<KeyElementType, ValueElementType>
{
public:
	using ElementType = typename TMap<KeyElementType, ValueElementType>::ElementType;

public:
	ValueElementType& Add(const KeyElementType& Key, const ValueElementType& Value)
	{
		const int32 Index = this->Elements.AddWithKeyHashFunc(
			ElementType(Key, Value),
			[](const ElementType& Pair) { return GetTypeHash(Pair.Key()); });

		return this->Elements[Index].Value();
	}

	void MultiFind(const KeyElementType& Key, TArray<ValueElementType>& OutValues) const
	{
		for (FBitArray::FSetBitIterator It(this->Elements.GetAllocationFlags()); It; ++It)
		{
			int32 Index = It.GetIndex();
			const ElementType& Pair = this->Elements[Index];

			if (Pair.Key() == Key)
			{
				OutValues.Add(Pair.Value());
			}
		}
	}

	int32 RemoveAll(const KeyElementType& Key)
	{
		int32 NumRemoved = 0;

		for (int32 Index = this->Elements.NumAllocated() - 1; Index >= 0; --Index)
		{
			if (this->Elements.IsValidIndex(Index) && this->Elements[Index].Key() == Key)
			{
				this->Elements.RemoveAt(Index);
				++NumRemoved;
			}
		}

		return NumRemoved;
	}
};

static_assert(sizeof(TMap<int32, int32>) == 0x50, "TMap layout broke: UE 4.0-5.x expects a single TSet member = 0x50 on x64");
static_assert(sizeof(TMultiMap<int32, int32>) == 0x50, "TMultiMap layout broke: must add no members over TMap");
