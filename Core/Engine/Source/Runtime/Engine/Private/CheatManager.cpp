#include "pch.h"

#include <Engine/Source/Runtime/Engine/Classes/GameFramework/CheatManager.h>
#include <Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h>

void UCheatManager::DestroyTarget()
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"DestroyTarget"));

	Call(Func);
}
