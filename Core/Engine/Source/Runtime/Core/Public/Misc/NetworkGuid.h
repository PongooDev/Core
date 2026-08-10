#pragma once
#include "pch.h"
#include "Core/Public/Version.h"

#include "Engine/Source/Runtime/Core/Public/Templates/TypeHash.h"

class FNetworkGUID
{
public:
	uint32 Value;

public:

	FNetworkGUID()
	{
		Value = 0;
	}

	FNetworkGUID(uint32 V)
	{
		Value = V;
	}

public:

	friend bool operator==(const FNetworkGUID& X, const FNetworkGUID& Y)
	{
		return X.Value == Y.Value;
	}

	friend bool operator!=(const FNetworkGUID& X, const FNetworkGUID& Y)
	{
		return X.Value != Y.Value;
	}

	friend uint32 GetTypeHash(const FNetworkGUID& Guid)
	{
		return GetTypeHash(Guid.Value);
	}

public:

	void BuildFromNetIndex(int32 StaticNetIndex)
	{
		Value = (StaticNetIndex << 1 | 1);
	}

	int32 ExtractNetIndex()
	{
		if (Value & 1)
		{
			return Value >> 1;
		}
		return 0;
	}
};