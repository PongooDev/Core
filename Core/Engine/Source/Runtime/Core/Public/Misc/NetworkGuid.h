#pragma once
#include "pch.h"
#include "Core/Public/Version.h"

class FNetworkGUID
{
public:
	union
	{
		uint32 Value;
		uint64 ObjectId;
	};

	static int32 GetSize()
	{
		if (Version::Engine_Version >= 5.3) {
			return 0x8;
		}
		else {
			return 0x4;
		}
	}

public:

	FNetworkGUID()
	{
		if (Version::Engine_Version >= 5.3) {
			ObjectId = 0;
		}
		else {
			Value = 0;
		}
	}

	FNetworkGUID(uint32 V)
	{
		if (Version::Engine_Version >= 5.3) {
			ObjectId = static_cast<uint64>(V);
		}
		else {
			Value = V;
		}
	}

public:

	friend bool operator==(const FNetworkGUID& X, const FNetworkGUID& Y)
	{
		if (Version::Engine_Version >= 5.3) {
			return X.ObjectId == Y.ObjectId;
		}
		else {
			return X.Value == Y.Value;
		}
	}

	friend bool operator!=(const FNetworkGUID& X, const FNetworkGUID& Y)
	{
		if (Version::Engine_Version >= 5.3) {
			return X.ObjectId != Y.ObjectId;
		}
		else {
			return X.Value != Y.Value;
		}
	}

public:

	void BuildFromNetIndex(int32 StaticNetIndex)
	{
		if (Version::Engine_Version >= 5.3) {
			ObjectId = ((uint64)StaticNetIndex << 1 | 1);
		}
		else {
			Value = (StaticNetIndex << 1 | 1);
		}
	}

	int32 ExtractNetIndex()
	{
		if (Version::Engine_Version >= 5.3) {
			return ((int32)ObjectId & 1) ? (int32)ObjectId >> 1 : 0;
		}
		else {
			if (Value & 1)
			{
				return Value >> 1;
			}
			return 0;
		}
	}
};