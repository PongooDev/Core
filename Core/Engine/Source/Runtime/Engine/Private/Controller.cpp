#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/GameFramework/Controller.h"

void AController::Possess(APawn* InPawn)
{
	static UFunction* Function = FindFunction(UKismetStringLibrary::Conv_StringToName(L"Possess"));
	if (Function) {
		static uintptr_t VTableIdx = GetVTableIndex(Function);

		void (*&PossessInternal)(AController*, APawn*) = decltype(PossessInternal)(VTable[VTableIdx]);
		PossessInternal(this, InPawn);
	}
}

APawn* AController::K2_GetPawn() const
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = FindFunction(UKismetStringLibrary::Conv_StringToName(L"K2_GetPawn"));

	struct Controller_K2_GetPawn
	{
	public:
		APawn* ReturnValue;
	};

	Controller_K2_GetPawn Parms{};

	const_cast<AController*>(this)->ProcessEvent(Func, &Parms);

	return Parms.ReturnValue;
}