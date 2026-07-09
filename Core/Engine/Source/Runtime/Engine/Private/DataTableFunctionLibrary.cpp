#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/DataTableFunctionLibrary.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

void UDataTableFunctionLibrary::EvaluateCurveTableRow(UCurveTable* CurveTable, FName RowName, float InXY, EEvaluateCurveTableResult::Type* OutResult, float* OutXY, const FString& ContextString)
{
	static class UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.DataTableFunctionLibrary.EvaluateCurveTableRow");

	GetDefaultObj()->Call(Func, CurveTable, RowName, InXY, OutResult, OutXY, ContextString);
}