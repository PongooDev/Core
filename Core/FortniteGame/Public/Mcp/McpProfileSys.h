#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

struct FBaseUrlContext {
public:
	DefineUnrealStruct(FBaseUrlContext);
public:
	uint8 Padding[0x90];
};

struct FPublicUrlContext : public FBaseUrlContext
{
public:
	DefineUnrealStruct(FPublicUrlContext);
};

struct FClientUrlContext : public FBaseUrlContext
{
public:
	DefineUnrealStruct(FClientUrlContext);
};

struct FDedicatedServerUrlContext : public FBaseUrlContext
{
public:
	DefineUnrealStruct(FDedicatedServerUrlContext);
};