#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Containers/Map.h"

// make more 1:1 in future

struct FFastArraySerializerItem
{
public:
	DefineUnrealStruct(FFastArraySerializerItem);
public:
	int32 ReplicationID;
	int32 ReplicationKey;
	int32 MostRecentArrayReplicationKey;
};

struct FFastArraySerializer
{
public:
	DefineUnrealStruct(FFastArraySerializer);
public:
	TMap<int32, int32> ItemMap;
	int32 IDCounter;
	int32 ArrayReplicationKey;
	char GuidReferencesMap[0x50];

	int32& GetCachedItems()
	{
		return GetFromOffset<int32>(this, Version::Engine_Version >= 4.23 ? 0xF8 : 0xA8);
	}

	int32& GetCachedItemsToConsiderForWriting()
	{
		return GetFromOffset<int32>(this, Version::Engine_Version >= 4.23 ? 0xFC : 0xAC);
	}

	/** This must be called if you add or change an item in the array */
	void MarkItemDirty(FFastArraySerializerItem& Item)
	{
		if (Item.ReplicationID == -1)
		{
			Item.ReplicationID = ++IDCounter;
			if (IDCounter == -1)
			{
				IDCounter++;
			}
		}

		Item.ReplicationKey++;
		MarkArrayDirty();
	}

	/** This must be called if you just remove something from the array */
	void MarkArrayDirty()
	{
		IncrementArrayReplicationKey();

		// Invalidate the cached item counts so that they're recomputed during the next write
		GetCachedItems() = -1;
		GetCachedItemsToConsiderForWriting() = -1;
	}

	void IncrementArrayReplicationKey()
	{
		ArrayReplicationKey++;
		if (ArrayReplicationKey == -1)
		{
			ArrayReplicationKey++;
		}
	}
};

struct FFortClientObservedStat : public FFastArraySerializerItem
{
public:
	DefineUnrealStruct(FFortClientObservedStat);
public:
	FName StatName;
	int32 StatValue;
};

struct FFortClientObservedStatArray : public FFastArraySerializer
{
public:
    DefineUnrealStruct(FFortClientObservedStatArray);
public:
	TArray<struct FFortClientObservedStat> ObservedStats;
};