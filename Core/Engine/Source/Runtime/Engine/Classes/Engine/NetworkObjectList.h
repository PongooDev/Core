#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/Core/Public/Containers/Set.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/WeakObjectPtr.h"
#include "Engine/Source/Runtime/Core/Public/Templates/SharedPointer.h"

class AActor;
class UNetConnection;

struct FNetworkObjectInfo
{
public:
	using FDormantConnectionSet = TSet<TWeakObjectPtr<UNetConnection>>;

private:
	enum class ELayout : uint8
	{
		Pre416,   // 4.12 - 4.15
		V416,     // 4.16
		V417,     // 4.17 - 4.18
		V419,     // 4.19 - 4.22
		V423,     // 4.23 - 4.24
		V425,     // 4.25 - 4.27
		V500,     // 5.0 +
	};

	static FORCEINLINE ELayout Layout()
	{
		const double V = Version::Engine_Version;

		if (V >= 5.00) return ELayout::V500;
		if (V >= 4.25) return ELayout::V425;
		if (V >= 4.23) return ELayout::V423;

		return ELayout::V419;
	}

	static constexpr uintptr_t SetSize = sizeof(FDormantConnectionSet);

	static constexpr uintptr_t Absent = 0;

public:
	AActor* Actor;

	static constexpr size_t MaxSize = 0x30 + 2 * SetSize + 0x08;

	uint8 Padding[MaxSize - sizeof(AActor*)];

public:
	static FORCEINLINE uintptr_t WeakActorOffset()
	{
		return Layout() >= ELayout::V419 ? 0x08 : Absent;
	}

	static FORCEINLINE uintptr_t NextUpdateTimeOffset()
	{
		switch (Layout())
		{
		case ELayout::V416:   return 0x08 + 2 * SetSize;
		case ELayout::Pre416:
		case ELayout::V417:   return 0x08;
		default:              return 0x10;
		}
	}

	static FORCEINLINE uintptr_t LastNetReplicateTimeOffset() { return NextUpdateTimeOffset() + 0x08; }
	static FORCEINLINE uintptr_t OptimalNetUpdateDeltaOffset() { return NextUpdateTimeOffset() + 0x10; }

	static FORCEINLINE uintptr_t LastNetUpdateTimeOffset()
	{
		const ELayout L = Layout();
		if (L == ELayout::Pre416 || L == ELayout::V500)
			return Absent;

		return NextUpdateTimeOffset() + 0x14;
	}

	static FORCEINLINE uintptr_t LastNetUpdateTimestampOffset()
	{
		if (Layout() < ELayout::V425)
			return Absent;

		return NextUpdateTimeOffset() + 0x18;
	}

	static FORCEINLINE uintptr_t DormantConnectionsOffset()
	{
		switch (Layout())
		{
		case ELayout::Pre416: return Absent;
		case ELayout::V416:   return 0x08;
		case ELayout::V417:   return 0x28;
		case ELayout::V419:   return 0x30;
		case ELayout::V423:   return 0x28;
		default:              return 0x30;
		}
	}

	static FORCEINLINE uintptr_t RecentlyDormantConnectionsOffset()
	{
		const uintptr_t Base = DormantConnectionsOffset();
		return Base ? Base + SetSize : Absent;
	}

	static FORCEINLINE uintptr_t FlagsOffset()
	{
		switch (Layout())
		{
		case ELayout::Pre416: return Absent;
		case ELayout::V416:   return 0x08 + 2 * SetSize + 0x18;
		case ELayout::V417:   return 0x20;
		case ELayout::V419:   return 0x28;
		default:              return DormantConnectionsOffset() + 2 * SetSize;
		}
	}

	static FORCEINLINE uintptr_t ForceRelevantFrameOffset()
	{
		return Layout() >= ELayout::V423 ? FlagsOffset() + 0x04 : Absent;
	}

public:
	DefineCustomProperty(TWeakObjectPtr<AActor>, WeakActor, FNetworkObjectInfo::WeakActorOffset());

	DefineCustomProperty(double, NextUpdateTime, FNetworkObjectInfo::NextUpdateTimeOffset());
	DefineCustomProperty(double, LastNetReplicateTime, FNetworkObjectInfo::LastNetReplicateTimeOffset());
	DefineCustomProperty(float, OptimalNetUpdateDelta, FNetworkObjectInfo::OptimalNetUpdateDeltaOffset());
	DefineCustomProperty(float, LastNetUpdateTime, FNetworkObjectInfo::LastNetUpdateTimeOffset());
	DefineCustomProperty(double, LastNetUpdateTimestamp, FNetworkObjectInfo::LastNetUpdateTimestampOffset());

	DefineCustomProperty(FDormantConnectionSet, DormantConnections, FNetworkObjectInfo::DormantConnectionsOffset());
	DefineCustomProperty(FDormantConnectionSet, RecentlyDormantConnections, FNetworkObjectInfo::RecentlyDormantConnectionsOffset());

	DefineCustomProperty(uint32, ForceRelevantFrame, FNetworkObjectInfo::ForceRelevantFrameOffset());

public:
	static FORCEINLINE int32 GetSize()
	{
		switch (Layout())
		{
		case ELayout::Pre416: return 0x20;
		case ELayout::V416:   return (int32)(0x08 + 2 * SetSize + 0x20);
		case ELayout::V417:   return (int32)(0x28 + 2 * SetSize);
		case ELayout::V419:   return (int32)(0x30 + 2 * SetSize);
		default:              return (int32)(FlagsOffset() + 0x08);
		}
	}

	static FNetworkObjectInfo* Allocate()
	{
		const size_t Actual = (size_t)GetSize();
		const size_t Size = Actual > sizeof(FNetworkObjectInfo) ? Actual : sizeof(FNetworkObjectInfo);

		FNetworkObjectInfo* Result = (FNetworkObjectInfo*)FMemory::Malloc(Size);
		if (Result)
			memset(Result, 0, Size);

		return Result;
	}

private:
	static FORCEINLINE bool FlagsAreBits() { return Layout() >= ELayout::V417; }

	FORCEINLINE bool GetFlag(int32 BitIndex) const
	{
		const uintptr_t Offset = FlagsOffset();
		if (!Offset || BitIndex < 0)
			return false;

		const uintptr_t Address = (uintptr_t)this + Offset;

		if (!FlagsAreBits())
			return *reinterpret_cast<bool*>(Address);

		return (*reinterpret_cast<uint32*>(Address) & (1u << BitIndex)) != 0;
	}

	FORCEINLINE void SetFlagBit(int32 BitIndex, bool bValue) const
	{
		const uintptr_t Offset = FlagsOffset();
		if (!Offset || BitIndex < 0)
			return;

		const uintptr_t Address = (uintptr_t)this + Offset;

		if (!FlagsAreBits())
		{
			*reinterpret_cast<bool*>(Address) = bValue;
			return;
		}

		uint32& Word = *reinterpret_cast<uint32*>(Address);
		if (bValue)
			Word |= (1u << BitIndex);
		else
			Word &= ~(1u << BitIndex);
	}

	static FORCEINLINE int32 BitOfPendingNetUpdate() { return Layout() == ELayout::Pre416 ? -1 : 0; }

	static FORCEINLINE int32 BitOfForceRelevantNextUpdate()
	{
		const ELayout L = Layout();
		return (L == ELayout::Pre416 || L == ELayout::V416 || L == ELayout::V500) ? -1 : 1;
	}

	static FORCEINLINE int32 BitOfDirtyForReplay()
	{
		const ELayout L = Layout();
		if (L < ELayout::V423) return -1;
		return L == ELayout::V500 ? 1 : 2;
	}

	static FORCEINLINE int32 BitOfSwapRolesOnReplicate()
	{
		const ELayout L = Layout();
		if (L < ELayout::V425) return -1;
		return L == ELayout::V500 ? 2 : 3;
	}

public:
	FORCEINLINE bool _GetbPendingNetUpdate() const { return GetFlag(BitOfPendingNetUpdate()); }
	FORCEINLINE void _SetbPendingNetUpdate(bool bValue) { SetFlagBit(BitOfPendingNetUpdate(), bValue); }

	FORCEINLINE bool _GetbForceRelevantNextUpdate() const { return GetFlag(BitOfForceRelevantNextUpdate()); }
	FORCEINLINE void _SetbForceRelevantNextUpdate(bool bValue) { SetFlagBit(BitOfForceRelevantNextUpdate(), bValue); }

	FORCEINLINE bool _GetbDirtyForReplay() const { return GetFlag(BitOfDirtyForReplay()); }
	FORCEINLINE void _SetbDirtyForReplay(bool bValue) { SetFlagBit(BitOfDirtyForReplay(), bValue); }

	FORCEINLINE bool _GetbSwapRolesOnReplicate() const { return GetFlag(BitOfSwapRolesOnReplicate()); }
	FORCEINLINE void _SetbSwapRolesOnReplicate(bool bValue) { SetFlagBit(BitOfSwapRolesOnReplicate(), bValue); }

	__declspec(property(get = _GetbPendingNetUpdate, put = _SetbPendingNetUpdate)) bool bPendingNetUpdate;
	__declspec(property(get = _GetbForceRelevantNextUpdate, put = _SetbForceRelevantNextUpdate)) bool bForceRelevantNextUpdate;
	__declspec(property(get = _GetbDirtyForReplay, put = _SetbDirtyForReplay)) bool bDirtyForReplay;
	__declspec(property(get = _GetbSwapRolesOnReplicate, put = _SetbSwapRolesOnReplicate)) bool bSwapRolesOnReplicate;
};

class FNetworkObjectList
{
public:
	typedef TSet<TSharedPtr<FNetworkObjectInfo>> FNetworkObjectSet;

	const FNetworkObjectSet& GetActiveObjects() const { return ActiveNetworkObjects; }

	TSharedPtr<FNetworkObjectInfo>* Add(AActor* const Actor, const FName NetDriverName);

	void Remove(AActor* const Actor);
public:
	FNetworkObjectSet AllNetworkObjects;
	FNetworkObjectSet ActiveNetworkObjects;
	FNetworkObjectSet ObjectsDormantOnAllConnections;

	TMap<TWeakObjectPtr<UNetConnection>, int32> NumDormantObjectsPerConnection;
};
