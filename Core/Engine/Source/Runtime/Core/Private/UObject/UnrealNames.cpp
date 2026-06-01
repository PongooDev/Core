#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

const FNameEntry* FName::GetComparisonNameEntry() const
{
	FNameEntry* (*GetComparisonNameEntryInternal)(const FName*) = decltype(GetComparisonNameEntryInternal)(ImageBase + Finder::FindFName_GetComparisonNameEntry());
	return GetComparisonNameEntryInternal(this);
}

FString FName::ToString() const
{
	FString Out = FString();

	if (Finder::FindFName_ToStringOut() != 0)
	{
		ToString(Out);
	}
	else {
		AppendString(Out);
	}

	return Out;
}

void FName::ToString(FString& Out) const
{
	static void (*ToStringInternal)(const FName*, FString&) = nullptr;
	if (!ToStringInternal)
	{
		uintptr_t addr = ImageBase + Finder::FindFName_ToStringOut();
		ToStringInternal = reinterpret_cast<void(*)(const FName*, FString&)>(addr);
	}

	ToStringInternal(this, Out);
}

void FName::AppendString(FString& Out) const
{
	auto AppendStringInternal = reinterpret_cast<void(*)(const FName*, FString&)>(
		ImageBase + Finder::FindFName_AppendString()
		);
	AppendStringInternal(this, Out);
}

int32 FName::Compare(const FName& Other) const
{
	auto CompareInternal = reinterpret_cast<int32(*)(const FName*, const FName*)>(
		ImageBase + Finder::FindFName_Compare()
		);
	return CompareInternal(this, &Other);
}

FName::FName(std::string& Name) {
	*this = UKismetStringLibrary::Conv_StringToName(Name);
}

FName::FName(FString& Name) {
	*this = UKismetStringLibrary::Conv_StringToName(Name);
}