#include "pch.h"
#include "FortniteGame/Public/AI/FortAIDirector.h"

void AFortAIDirector::Activate()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("Activate");

	Call(Func);
}