#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Engine/Classes/Engine/NetConnection.h"

class UChildConnection : public UNetConnection
{
public:
	DefineUnrealClass(UChildConnection);

	DefineUProperty(UNetConnection*, Parent);
};