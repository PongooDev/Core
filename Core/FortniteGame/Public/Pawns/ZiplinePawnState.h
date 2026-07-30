#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/Math/Vector.h"

class AFortAthenaZipline;

struct FZiplinePawnState {
public:
	DefineUnrealStruct(FZiplinePawnState);

	DefineStructProperty(AFortAthenaZipline*, Zipline);
	DefineStructProperty(bool, bIsZiplining);
	DefineStructProperty(bool, bJumped);
	DefineStructProperty(int32, AuthoritativeValue);
	DefineStructProperty(FVector, SocketOffset);
	DefineStructProperty(float, TimeZipliningBegan);
	DefineStructProperty(float, TimeZipliningEndedFromJump);
public:
	uint8 Padding[0x28];
};
