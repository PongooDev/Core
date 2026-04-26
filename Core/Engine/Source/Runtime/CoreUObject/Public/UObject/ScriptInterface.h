#pragma once
#include "pch.h"

#include "UnrealType.h"

class FScriptInterface
{
public:
	UObject* ObjectPointer;

	void* InterfacePointer;
};

template<class InterfaceType>
class TScriptInterface : public FScriptInterface
{
public:
	TScriptInterface()
	{
		ObjectPointer = nullptr;
		InterfacePointer = nullptr;
	}

	InterfaceType* operator->() const
	{
		return (InterfaceType*)InterfacePointer;
	}

	operator bool() const
	{
		return InterfacePointer != nullptr;
	}
};