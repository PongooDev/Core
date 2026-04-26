#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

class FOutputDevice
{
public:
	void** VTable;
public:
	bool bSuppressEventTag;
	bool bAutoEmitLineTerminator;
};

class FFrame : public FOutputDevice
{
public:
	UFunction* Node;
	UObject* Object;
	uint8* Code;
	uint8* Locals;
	void* MostRecentProperty;
	uint8_t* MostRecentPropertyAddress;
	uint8_t _Padding1[0x40];
	const UField* PropertyChainForCompiledIn;

public:
	UFunction* GetCurrentNativeFunction()
	{
		return *(UFunction**)(__int64(this) + ServerOffsets::FFrame__CurrentNativeFunction);
	}

	void StepCompiledIn(void* const Result = nullptr)
	{
		if (Code)
		{
			((void (*)(FFrame*, UObject*, void* const)) (ImageBase + Finder::FindStep()))(this, Object, Result);
		}
		else
		{
			const UField* _Prop = *(const UField**)(__int64(this) + ServerOffsets::FFrame__PropertyChainForCompiledIn);
			if (_Prop)
			{
				*(const UField**)(__int64(this) + ServerOffsets::FFrame__PropertyChainForCompiledIn) = *(const UField**)(__int64(_Prop) + ServerOffsets::FFrame__Next);
				((void (*)(FFrame*, void* const, const UField*)) (ImageBase + Finder::FindStepExplicitProperty()))(this, Result, _Prop);
			}
		}
	}

	void* StepCompiledInRefInternal(void* _Tm)
	{
		MostRecentPropertyAddress = nullptr;
		if (Version::Fortnite_Version >= 20.20)
			*(void**)(__int64(this) + 0x40) = nullptr;

		if (Code)
		{
			((void (*)(FFrame*, UObject*, void* const)) (ImageBase + Finder::FindStep()))(this, Object, _Tm);
		}
		else
		{
			const UField* _Prop = *(const UField**)(__int64(this) + ServerOffsets::FFrame__PropertyChainForCompiledIn);
			*(const UField**)(__int64(this) + ServerOffsets::FFrame__PropertyChainForCompiledIn) = *(const UField**)(__int64(_Prop) + ServerOffsets::FFrame__Next);
			((void (*)(FFrame*, void* const, const UField*)) (ImageBase + Finder::FindStepExplicitProperty()))(this, _Tm, _Prop);
		}

		return MostRecentPropertyAddress ? MostRecentPropertyAddress : _Tm;
	}

	template <typename T>
	T& StepCompiledInRef()
	{
		T TempVal{};
		return *(T*)StepCompiledInRefInternal(&TempVal);
	}

	void IncrementCode()
	{
		Code = (uint8_t*)(__int64(Code) + (bool)Code);
	}
};

static inline void* _NpFH = nullptr;

#define callOG(_Tr, _Fn, _Th, ...) ([&](){ _Fn->Func = _Th##_OG; _Tr->_Th(##__VA_ARGS__); _Fn->Func = _Th##_; })()
#define callOGWithRet(_Tr, _Fn, _Th, ...) ([&](){ _Fn->Func = _Th##_OG; auto _Rt = _Tr->_Th(##__VA_ARGS__); _Fn->Func = _Th##_; return _Rt; })()

template <typename _Ot = void*>
__forceinline static void ExecHook(UFunction* _Fn, void* _Detour, _Ot& _Orig = _NpFH)
{
	if (!_Fn)
		return;
	if (!std::is_same_v<_Ot, void*>)
		_Orig = (_Ot)_Fn->Func;

	_Fn->Func = _Detour;
	Log("Hooked function: " + _Fn->GetName().ToString());
}

template <typename _Ot = void*>
__forceinline static void ExecHook(const std::string& _Name, void* _Detour, _Ot& _Orig = _NpFH)
{
	UObject* _Fn = FUObjectArray::FindObject(_Name);
	if (!_Fn)
	{
		Log("Failed to find function: " + _Name);
		return;
	}
	ExecHook((UFunction*)_Fn, _Detour, _Orig);
}