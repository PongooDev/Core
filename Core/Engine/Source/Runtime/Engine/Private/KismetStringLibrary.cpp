#include "pch.h"
#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

FName UKismetStringLibrary::Conv_StringToName(const FString& InString)
{
	if (Finder::FindUKismetStringLibrary_Conv_StringToName() != 0) {
		FName(*Conv_StringToNameInternal)(const FString&) = decltype(Conv_StringToNameInternal)(ImageBase + Finder::FindUKismetStringLibrary_Conv_StringToName());
		return Conv_StringToNameInternal(InString);
	}
	else {
		static UFunction* Func = nullptr;

		if (Func == nullptr)
			Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Conv_StringToName");

		if (!Func) {
			Log("UKismetStringLibrary::Conv_StringToName: Failed to find function!");
			return FName();
		}

		return GetDefaultObj()->Call<FName>(Func, InString);
	}
}

FString UKismetStringLibrary::ToUpper(const FString& SourceString)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.ToUpper");

	return GetDefaultObj()->Call<FString>(Func, SourceString);
}

bool UKismetStringLibrary::Contains(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Contains");

	return GetDefaultObj()->Call<bool>(Func, SearchIn, Substring, bUseCase, bSearchFromEnd);
}

int32 UKismetStringLibrary::FindSubstring(const class FString& SearchIn, const class FString& Substring, bool bUseCase, bool bSearchFromEnd, int32 StartPosition)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.FindSubstring");

	return GetDefaultObj()->Call<int32>(Func, SearchIn, Substring, bUseCase, bSearchFromEnd, StartPosition);
}

FString UKismetStringLibrary::Left(const class FString& SourceString, int32 Count)
{
	static UFunction* Func = nullptr;

	if (Func == nullptr)
		Func = StaticClass()->GetFunction("Function /Script/Engine.KismetStringLibrary.Left");

	return GetDefaultObj()->Call<FString>(Func, SourceString, Count);
}