#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"

typedef int32 NAME_INDEX;

#define NAME_NO_NUMBER_INTERNAL	0

enum class ENameCase : uint8
{
	CaseSensitive,
	IgnoreCase,
};

struct FNameEntry {

};

class FName
{
public:
	const FNameEntry* GetComparisonNameEntry() const;

	FString ToString() const;

	void ToString(FString& Out) const;

	void AppendString(FString& Out) const;

	FORCEINLINE bool IsEqual(const FName& Other, const ENameCase CompareMethod = ENameCase::IgnoreCase, const bool bCompareNumber = true) const
	{
		return ComparisonIndex == Other.ComparisonIndex
			&& (!bCompareNumber || Number == Other.Number);
	}

	FORCEINLINE bool operator==(const FName& Other) const
	{
		return ComparisonIndex == Other.ComparisonIndex && Number == Other.Number;
	}
	FORCEINLINE bool operator!=(const FName& Other) const
	{
		return !(*this == Other);
	}

	FORCEINLINE bool operator<(const FName& Other) const
	{
		return Compare(Other) < 0;
	}


	FORCEINLINE bool operator>(const FName& Other) const
	{
		return Compare(Other) > 0;
	}

	FORCEINLINE bool IsNone() const
	{
		return ComparisonIndex == 0 && Number == 0;
	}

	int32 Compare(const FName& Other) const;

	FORCEINLINE FName(const FName& Other, int32 InNumber)
		: ComparisonIndex(Other.ComparisonIndex)
		, Number(InNumber)
	{
	}

	FORCEINLINE FName(const NAME_INDEX InComparisonIndex, const int32 InNumber)
		: ComparisonIndex(InComparisonIndex)
		, Number(InNumber)
	{
	}

	FORCEINLINE FName()
		: ComparisonIndex(0)
		, Number(NAME_NO_NUMBER_INTERNAL)
	{
	}
public:
	NAME_INDEX ComparisonIndex;

	uint32 Number;
public:
	// Custom
	FName(std::string& Name);

	FName(FString& Name);

	operator std::string() const;
};