#pragma once
#include "pch.h"
#include "Core/Public/Finder.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/Core/Public/Serialization/BitWriter.h"
#include "Engine/Source/Runtime/Core/Public/Misc/NetworkGuid.h"

class FOutBunch;
class INetDeltaBaseState;
struct FStringAssetReference;
class FNetworkGUID;

class UPackageMap : public UObject {
public:
	bool WriteObject(FArchive& Ar, UObject* InOuter, FNetworkGUID NetGUID, FString ObjName)
	{
		bool (*WriteObjectInternal)(UPackageMap*, FArchive&, UObject*, FNetworkGUID, FString) = decltype(WriteObjectInternal)(ImageBase + Finder::FindUPackageMap_WriteObject());
		return WriteObjectInternal(this, Ar, InOuter, NetGUID, ObjName);
	}
};

class FNetBitWriter : public FBitWriter
{
public:
};

struct FPacketIdRange
{
	FPacketIdRange(int32 _First, int32 _Last) : First(_First), Last(_Last) {}
	FPacketIdRange(int32 PacketId) : First(PacketId), Last(PacketId) {}
	FPacketIdRange() : First(INDEX_NONE), Last(INDEX_NONE) {}
	int32	First;
	int32	Last;

	bool InRange(int32 PacketId)
	{
		return (First <= PacketId && PacketId <= Last);
	}
};