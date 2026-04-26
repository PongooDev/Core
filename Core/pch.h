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

#include "../includes/memcury.h"
#include "../includes/MinHook/MinHook.h"

#include "Core/Public/Offsets.h"
#include "Core/Public/Configuration.h"
#include "Core/Public/Finder.h"

inline uintptr_t ImageBase = (uintptr_t)GetModuleHandleA(0);

#ifndef INDEX_NONE
#define INDEX_NONE -1
#endif

inline void Log(const std::string& msg)
{
	static bool firstCall = true;

	FCoreConfig& Config = ConfigurationManager::GetConfig();
	std::string FileName = Config.bIsClient ? "Client_log.txt" : "Server_log.txt";
	std::string LogType = Config.bIsClient ? "Client" : "Server";

	if (firstCall)
	{
		std::ofstream logFile(FileName, std::ios::trunc);
		if (logFile.is_open())
		{
			logFile << "Log" + LogType + ": Log file initialized!\n";
			logFile.close();
		}
		firstCall = false;
	}

	std::ofstream logFile(FileName, std::ios::app);
	if (logFile.is_open())
	{
		logFile << "Log" + LogType + ": " << msg << std::endl;
		logFile.close();
	}

	std::cout << "Log" + LogType + ": " << msg << std::endl;
}

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

uintptr_t GetVTableIndex(class UObject* Object, class UFunction* Func);
void HookVTable(class UObject* Object, class UFunction* Func, void* Detour, void** Original = nullptr);
void HookVTableIdx(void* Base, int Idx, void* Detour, void** OG = nullptr);
void HookEveryVTableIdx(UClass* Base, int Idx, void* Detour);

#endif //PCH_H
