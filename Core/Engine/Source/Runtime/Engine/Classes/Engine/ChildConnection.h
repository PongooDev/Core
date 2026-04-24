#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

class UChildConnection : public UNetConnection
{
	DefineUnrealClass(UChildConnection);

	DefineUProperty(UNetConnection*, Parent);

public:
	int32 IsNetReady(bool Saturate)
	{
		return Parent->IsNetReady(Saturate);
	}
};