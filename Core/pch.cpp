// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"
#include "Core/Public/Utils.h"
#include "Gui/Public/Gui.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

void Log(const std::string& msg)
{
	FCoreConfig& Config = ConfigurationManager::GetConfig();
	if (Config.bIsProd)
		return;

	const bool bClient = Config.bIsClient;
	const char* LogType = bClient ? "Client" : "Server";

	static FILE* Files[2] = { nullptr, nullptr };
	FILE*& LogFile = Files[bClient ? 1 : 0];

	if (!LogFile)
	{
		wchar_t Path[MAX_PATH] = {};
		const DWORD Length = GetModuleFileNameW(nullptr, Path, MAX_PATH);

		if (Length > 0 && Length < MAX_PATH)
		{
			if (wchar_t* LastSlash = wcsrchr(Path, L'\\'))
				*(LastSlash + 1) = L'\0';
			else
				Path[0] = L'\0';
		}
		else
		{
			Path[0] = L'\0';
		}

		wcsncat_s(Path, bClient ? L"Client_log.txt" : L"Server_log.txt", _TRUNCATE);

		_wfopen_s(&LogFile, Path, L"w");
		if (LogFile)
			fprintf(LogFile, "Log%s: Log file initialized!\n", LogType);
	}

	if (LogFile)
	{
		fprintf(LogFile, "Log%s: %s\n", LogType, msg.c_str());

		fflush(LogFile);
	}

	Gui::PushLog("Log" + std::string(LogType) + ": " + msg);

	if (!Gui::IsCapturingConsole())
		std::cout << "Log" << LogType << ": " << msg << '\n';
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

	int* DispPtr = nullptr;
	if (bHasValidate)
	{
		DispPtr = Memcury::Scanner(Func->Func)
			.ScanFor({ 0xFF, 0x90 }, true, 1)
			.AbsoluteOffset(2)
			.GetAs<int*>();
	}
	else
	{
		DispPtr = Memcury::Scanner(Func->Func)
			.ScanFor({ 0xFF, 0x90 })
			.AbsoluteOffset(2)
			.GetAs<int*>();
	}

	if (!DispPtr)
	{
		Log("GetVTableIndex: no virtual call found in exec of " + FuncName);
		return -1;
	}

	VirtualFuncIdx = *DispPtr;

	if (VirtualFuncIdx <= 0 || VirtualFuncIdx >= 0x8000 || (VirtualFuncIdx & 7) != 0)
	{
		Log("GetVTableIndex: implausible displacement " + std::format("0x{:X}", VirtualFuncIdx) + " in exec of " + FuncName);
		return -1;
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
	if (VTableIndex <= 0 || VTableIndex >= 0x4000)
	{
		Log("HookVTable: refusing out of range index " + std::format("0x{:X}", VTableIndex));
		return;
	}

	DWORD oldProtection;
	void** VTable = *(void***)Object;

	if (!IsExecutableAddress((uintptr_t)VTable[VTableIndex]))
	{
		Log("HookVTable: slot " + std::format("0x{:X}", VTableIndex) + " does not point at code, refusing to hook");
		return;
	}

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

	if (!IsExecutableAddress((uintptr_t)VTable[Idx]))
	{
		Log("HookVTableIdx: slot " + std::format("0x{:X}", Idx) + " does not point at code, refusing to hook");
		return;
	}

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

	if (Idx <= 0 || Idx >= 0x4000)
	{
		Log("HookEveryVTableIdx: refusing out of range index " + std::format("0x{:X}", Idx));
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
	if (VTableIndex <= 0 || VTableIndex >= 0x4000)
	{
		Log("HookEveryVTable: refusing out of range index " + std::format("0x{:X}", VTableIndex));
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
