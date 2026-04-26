// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UnrealType.h"

uintptr_t GetVTableIndex(class UObject* Object, class UFunction* Func) {
	if (!Object || !Func)
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

	if (FuncName == "ReadyToStartMatch" || FuncName == "SpawnDefaultPawnFor"
		|| FuncName == "HandleStartingNewPlayer")
	{
		VTableIndex = VirtualFuncIdx / 8;
	}
	else
	{
		VTableIndex = (VirtualFuncIdx / 8) + 1;
	}

	return VTableIndex;
}

void HookVTable(UObject* Object, UFunction* Func, void* Detour, void** Original)
{
	if (!Object || !Func || !Detour)
	{
		Log("Invalid parameters for HookVTable");
		return;
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

	if (FuncName == "ReadyToStartMatch" || FuncName == "SpawnDefaultPawnFor"
		|| FuncName == "HandleStartingNewPlayer") //idfk????
	{
		VTableIndex = VirtualFuncIdx / 8;
	}
	else
	{
		VTableIndex = (VirtualFuncIdx / 8) + 1;
	}

	std::stringstream ss;
	ss << FuncName << " (Index): 0x" << std::uppercase << std::hex << VTableIndex;
	Log(ss.str());

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

void HookEveryVTableIdx(UClass* Base, int Idx, void* Detour)
{
	if (!Base || !Detour)
	{
		Log("Invalid parameters for HookEveryVTableIdx");
		return;
	}

	for (int i = 0; i < FUObjectArray::Num(); i++) {
		FUObjectItem* ObjectItem = FUObjectArray::IndexToObject(i);
		if (!ObjectItem)
			continue;

		UObject* Object = (UObject*)ObjectItem->Object;
		if (!Object)
			continue;

		if (Object && Object->IsA(Base) && Object->IsDefaultObject())
		{
			HookVTableIdx(Object, Idx, Detour, nullptr);
			Log("Hooked " + Object->GetName().ToString() + " at index " + std::to_string(Idx));
		}
	}
}

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.
