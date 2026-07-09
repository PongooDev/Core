#include "pch.h"
#include "FortniteGame/Public/FortSet/FortHealthSet.h"

void UFortHealthSet::OnRep_CurrentShield()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_CurrentShield");

	Call(Func);
}

void UFortHealthSet::OnRep_Health()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Health");

	Call(Func);
}

void UFortHealthSet::OnRep_MaxHealth()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_MaxHealth");

	Call(Func);
}

void UFortHealthSet::OnRep_Shield()
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction("OnRep_Shield");

	Call(Func);
}