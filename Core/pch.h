// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

// add headers that you want to pre-compile here
#include "framework.h"
#include <cstdint>
#include <string>
#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <map>
#include <format>
#include <print>
#include <array>
#include <minmax.h>
#include <TlHelp32.h>
#include <sstream>
#include <winternl.h>
#include <cstdlib>
#include <cmath>
#include <intrin.h>
#include <windows.h>
#include <psapi.h>
#include <unordered_map>
#include <numeric>

#include "../includes/memcury.h"
#include "../includes/MinHook/MinHook.h"

#include "Core/Public/Offsets.h"
#include "Core/Public/Configuration.h"
#include "Core/Public/Finder.h"

#pragma comment(lib, "ntdll.lib")

inline uintptr_t ImageBase = (uintptr_t)GetModuleHandleA(0);

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

inline bool GCanUseEngineLog = false;
void Log(const std::string& msg);

template<typename T>
static inline T& GetFromOffset(const void* base, size_t offset)
{
	return *reinterpret_cast<T*>(const_cast<uint8_t*>(reinterpret_cast<const uint8_t*>(base) + offset));
}

inline int True() {
	return 1;
}

inline int False() {
	return 0;
}

static void* RetNullptr() {
	return nullptr;
}

static inline void* (*CheckThisOG)(void* a1, void* a2);
static void* CheckThis(void* a1, void* a2) {
    if (!a1) {
		return nullptr;
    }

	return CheckThisOG(a1, a2);
}

static void RetNull() {}

class UObject;

using StaticFindObjectFn = void* (__cdecl*)(class UClass*, UObject* Package, const wchar_t* OrigInName, bool ExactClass);
using StaticLoadObjectFn = void* (__cdecl*)(class UClass*, UObject* InOuter, const TCHAR* Name, const TCHAR* Filename, uint32_t LoadFlags, UObject* Sandbox, bool bAllowObjectReconciliation, void*);

__forceinline static UObject* StaticFindObjectOG(const wchar_t* ObjectPath, const UClass* Class)
{
	auto StaticFindObjectInternal = (UObject * (*)(const UClass*, UObject*, const wchar_t*, bool)) (ImageBase + Finder::FindStaticFindObject());
	return StaticFindObjectInternal(Class, nullptr, ObjectPath, false);
}

__forceinline static UObject* StaticLoadObjectOG(const wchar_t* ObjectPath, const UClass* InClass, UObject* Outer = nullptr)
{
	auto StaticLoadObjectInternal = (UObject * (*)(const UClass*, UObject*, const wchar_t*, const wchar_t*, uint32_t, UObject*, bool, void*)) (ImageBase + Finder::FindStaticLoadObject());
	return StaticLoadObjectInternal(InClass, Outer, ObjectPath, nullptr, 0, nullptr, false, nullptr);
}

template <typename T>
inline T* StaticFindObject(std::wstring ObjectName)
{
	return (T*)StaticFindObjectOG(ObjectName.c_str(), T::StaticClass());
}

inline UObject* StaticFindObject(const std::wstring& ObjectName, const UClass* InClass = nullptr)
{
	return StaticFindObjectOG(ObjectName.c_str(), InClass);
}

template<typename T = UObject>
inline T* StaticLoadObject(const std::string& Name)
{
	auto ConvName = std::wstring(Name.begin(), Name.end());

	T* Object = StaticFindObject<T>(ConvName);

	if (!Object)
	{
		Object = (T*)StaticLoadObjectOG(ConvName.c_str(), T::StaticClass());
	}

	return Object;
}

inline UObject* StaticLoadObject(const std::string& Name, const UClass* InClass = nullptr, UObject* Outer = nullptr)
{
	auto ConvName = std::wstring(Name.begin(), Name.end());
	UObject* Object = StaticFindObject(ConvName);
	if (!Object)
	{
		Object = StaticLoadObjectOG(ConvName.c_str(), InClass, Outer);
	}
	return Object;
}

uintptr_t GetVTableIndex(class UFunction* Func);
void HookVTable(class UObject* Object, class UFunction* Func, void* Detour, void** Original = nullptr);
void HookVTableIdx(void* Base, int Idx, void* Detour, void** OG = nullptr);
void HookEveryVTableIdx(UClass* Base, int Idx, void* Detour, void** OG = nullptr, bool bSilent = true);
void HookEveryVTable(UClass* Base, class UFunction* Func, void* Detour, void** OG = nullptr, bool bSilent = true);
void CreateVTableOriginal(void* Base, int Idx, void** Original);
void CreateVTableOriginal(void* Base, class UFunction* Func, void** Original);
uintptr_t GetOffsetFromVTable(class UObject* Object, class UFunction* Func);
uintptr_t GetOffsetFromVTable(class UObject* Object, int Idx);

inline bool PatchBytes(void* address, const void* bytes, size_t size)
{
    if (!address || !bytes || size == 0)
        return false;

    DWORD oldProtect{};
    if (!VirtualProtect(address, size, PAGE_EXECUTE_READWRITE, &oldProtect))
        return false;

    std::memcpy(address, bytes, size);
    FlushInstructionCache(GetCurrentProcess(), address, size);

    DWORD temp{};
    VirtualProtect(address, size, oldProtect, &temp);
    return true;
}

inline bool PatchByte(void* address, uint8_t value)
{
    return PatchBytes(address, &value, sizeof(value));
}

inline bool PatchByte(uintptr_t address, uint8_t value)
{
    return PatchByte(reinterpret_cast<void*>(address), value);
}

inline bool PatchByteChecked(uintptr_t address, uint8_t expected, uint8_t value)
{
    auto ptr = reinterpret_cast<uint8_t*>(address);
    if (!ptr || *ptr != expected)
        return false;

    return PatchByte(address, value);
}

inline bool NopBytes(void* address, size_t count)
{
    if (!address || count == 0)
        return false;

    std::vector<uint8_t> nops(count, 0x90);
    return PatchBytes(address, nops.data(), nops.size());
}

inline bool NopBytes(uintptr_t address, size_t count)
{
    return NopBytes(reinterpret_cast<void*>(address), count);
}

inline bool PatchCall(uintptr_t callSite, void* newTarget)
{
    if (!callSite || !newTarget)
        return false;

    int32_t relOffset = (int32_t)((uintptr_t)newTarget - (callSite + 5));

    uint8_t patch[5];
    patch[0] = 0xE8;
    memcpy(&patch[1], &relOffset, sizeof(relOffset));

    return PatchBytes(reinterpret_cast<void*>(callSite), patch, sizeof(patch));
}

inline bool PatchCallFar(uintptr_t callSite, void* newTarget)
{
    uintptr_t base = callSite & ~0xFFFFull;
    void* trampoline = nullptr;

    for (uintptr_t addr = base; addr > base - 0x80000000ull; addr -= 0x10000)
    {
        trampoline = VirtualAlloc((void*)addr, 16, MEM_COMMIT | MEM_RESERVE, PAGE_EXECUTE_READWRITE);
        if (trampoline) break;
    }

    if (!trampoline)
        return false;

    uint8_t jmp[] = {
        0xFF, 0x25, 0x00, 0x00, 0x00, 0x00,
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    };
    memcpy(&jmp[6], &newTarget, sizeof(newTarget));
    memcpy(trampoline, jmp, sizeof(jmp));

    return PatchCall(callSite, trampoline);
}

struct _Pad_0x10
{
    uint8_t Padding[0x10];
};

struct _Pad_0x18
{
    uint8_t Padding[0x18];
};

static bool IsAddressInModule(uintptr_t addr, HMODULE module)
{
    MODULEINFO mi{};
    if (!GetModuleInformation(GetCurrentProcess(), module, &mi, sizeof(mi)))
        return false;

    const uintptr_t base = reinterpret_cast<uintptr_t>(mi.lpBaseOfDll);
    const uintptr_t end = base + mi.SizeOfImage;
    return addr >= base && addr < end;
}

#endif //PCH_H
