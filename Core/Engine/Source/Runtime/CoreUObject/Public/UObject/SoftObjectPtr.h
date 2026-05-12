#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "SoftObjectPath.h"
#include "WeakObjectPtr.h"

template<typename TObjectID>
class TPersistentObjectPtr
{
public:
	FWeakObjectPtr WeakPtr;
	int32 TagAtLastTest;
	TObjectID ObjectID;

public:
	class UObject* Get() const
	{
		return WeakPtr.Get();
	}
};

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
};

template<typename UEType>
class TSoftObjectPtr : public FSoftObjectPtr
{
public:
	UEType* Get() const
	{
		return static_cast<UEType*>(TPersistentObjectPtr::Get());
	}
};