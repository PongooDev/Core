// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"
#include "Core/Public/Utils.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

void Log(const std::string& msg)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bIsProd)
		return;

	std::string FileName = Config.bIsClient ? "Client_log.txt" : "Server_log.txt";
	std::string LogType = Config.bIsClient ? "Client" : "Server";

	static bool firstCallClient = true;
	static bool firstCallServer = true;

	bool& firstCall = Config.bIsClient ? firstCallClient : firstCallServer;

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

uintptr_t GetVTableIndex(class UFunction* Func) {
	if (!Func)
	{
		Log("Invalid parameters for GetVTableIndex");
		return -1;
	}
	std::string FuncName = Func->GetFName().ToString().ToString();

	std::string ValidateString = std::string(FuncName.c_str()) + "_Validate";
	bool bHasValidate = Memcury::Scanner::FindStringRef(ValidateString.c_str()).Get() != 0;

	int VirtualFuncIdx = 0;

	if (bHasValidate)
	{
		VirtualFuncIdx = *Memcury::Scanner(Func->Func)
			.ScanFor({ 0xFF, 0x90 }, true, 1)
			.AbsoluteOffset(2)
			.GetAs<int*>();
	}
	else
	{
		VirtualFuncIdx = *Memcury::Scanner(Func->Func)
			.ScanFor({ 0xFF, 0x90 })
			.AbsoluteOffset(2)
			.GetAs<int*>();
	}

	int VTableIndex = 0;

	if (FuncName.contains("Server"))
	{
		VTableIndex = (VirtualFuncIdx / 8) + 1;
	}
	else
	{
		VTableIndex = VirtualFuncIdx / 8;
	}

	std::stringstream ss;
	ss << FuncName << " (Index): 0x" << std::uppercase << std::hex << VTableIndex;
	Log(ss.str());
	return VTableIndex;
}

void HookVTable(UObject* Object, UFunction* Func, void* Detour, void** Original)
{
	if (!Object || !Func || !Detour)
	{
		Log("Invalid parameters for HookVTable");
		return;
	}
	int VTableIndex = GetVTableIndex(Func);

	DWORD oldProtection;
	void** VTable = *(void***)Object;

	if (Original)
	{
		*Original = VTable[VTableIndex];
	}

	VirtualProtect(&VTable[VTableIndex], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);
	VTable[VTableIndex] = Detour;
	VirtualProtect(&VTable[VTableIndex], sizeof(void*), oldProtection, nullptr);
}

void HookVTableIdx(void* Base, int Idx, void* Detour, void** OG)
{
	if (!Base || !Detour)
	{
		Log("Invalid parameters for HookVTableIdx");
		return;
	}
	DWORD oldProtection;

	void** VTable = *(void***)Base;

	if (OG)
	{
		*OG = VTable[Idx];
	}

	VirtualProtect(&VTable[Idx], sizeof(void*), PAGE_EXECUTE_READWRITE, &oldProtection);

	VTable[Idx] = Detour;

	VirtualProtect(&VTable[Idx], sizeof(void*), oldProtection, NULL);
}

void HookEveryVTableIdx(UClass* Base, int Idx, void* Detour, void** OG, bool bSilent)
{
	if (!Base || !Detour)
	{
		Log("Invalid parameters for HookEveryVTableIdx");
		return;
	}

	bool OGSet = false;

	for (int i = 0; i < FUObjectArray::Num(); i++) {
		FUObjectItem* ObjectItem = FUObjectArray::IndexToObject(i);
		if (!ObjectItem)
			continue;

		UObject* Object = (UObject*)ObjectItem->Object;
		if (!Object)
			continue;

		if (Object && Object->IsA(Base) && Object->IsDefaultObject())
		{
			HookVTableIdx(Object, Idx, Detour, OGSet ? nullptr : (LPVOID*)OG);
			if (!OGSet) OGSet = true;
			if (!bSilent) {
				Log("Hooked " + Object->GetName().ToString() + " at index " + std::to_string(Idx));
			}
		}
	}
}

void HookEveryVTable(UClass* Base, class UFunction* Func, void* Detour, void** OG, bool bSilent)
{
	if (!Base || !Detour || !Func)
	{
		Log("Invalid parameters for HookEveryVTableIdx");
		return;
	}
	int VTableIndex = GetVTableIndex(Func);

	bool OGSet = false;

	for (int i = 0; i < FUObjectArray::Num(); i++) {
		FUObjectItem* ObjectItem = FUObjectArray::IndexToObject(i);
		if (!ObjectItem)
			continue;

		UObject* Object = (UObject*)ObjectItem->Object;
		if (!Object)
			continue;

		if (Object && Object->IsA(Base) && Object->IsDefaultObject())
		{
			HookVTableIdx(Object, VTableIndex, Detour, OGSet ? nullptr : (LPVOID*)OG);
			if (!OGSet) OGSet = true;
			if (!bSilent) {
				Log("Hooked " + Func->GetName().ToString() + " in " + Object->GetName().ToString() + " at index " + std::to_string(VTableIndex));
			}
		}
	}
}

void CreateVTableOriginal(void* Base, int Idx, void** Original) {
	if (!Base)
	{
		Log("Invalid parameters for CreateVTableOriginal");
		return;
	}
	DWORD oldProtection;

	void** VTable = *(void***)Base;

	if (Original)
	{
		*Original = VTable[Idx];
	}
}

void CreateVTableOriginal(void* Base, class UFunction* Func, void** Original) {
	if (!Base || !Func)
	{
		Log("Invalid parameters for CreateVTableOriginal");
		return;
	}
	int VTableIndex = GetVTableIndex(Func);

	void** VTable = *(void***)Base;

	if (Original)
	{
		*Original = VTable[VTableIndex];
	}
}

uintptr_t GetOffsetFromVTable(UObject* Object, UFunction* Func)
{
	if (!Object || !Func)
	{
		Log("Invalid parameters for GetOffsetFromVTable");
		return static_cast<uintptr_t>(-1);
	}

	int VTableIndex = GetVTableIndex(Func);
	void** VTable = Object->VTable;
	uintptr_t Offset = reinterpret_cast<uintptr_t>(VTable[VTableIndex]);

	std::stringstream ss;
	ss << "Offset for " << Func->GetName().ToString()
		<< ": 0x" << std::hex << (Offset - ImageBase);
	Log(ss.str());

	return Offset;
}

uintptr_t GetOffsetFromVTable(UObject* Object, int Idx)
{
	if (!Object)
	{
		Log("Invalid parameters for GetOffsetFromVTable");
		return static_cast<uintptr_t>(-1);
	}

	void** VTable = Object->VTable;
	uintptr_t Offset = reinterpret_cast<uintptr_t>(VTable[Idx]);

	std::stringstream ss;
	ss << "Offset for index " << Idx
		<< ": 0x" << std::hex << (Offset - ImageBase);
	Log(ss.str());

	return Offset;
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
