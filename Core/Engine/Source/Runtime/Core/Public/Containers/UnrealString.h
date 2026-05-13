#pragma once
#include "pch.h"

#include "Array.h"
#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

class FString
{
private:
	typedef TArray<TCHAR> DataType;
	DataType Data;

public:
	using ElementType = TCHAR;

	FORCEINLINE FString() : Data()
	{
	}

	FORCEINLINE FString(const FString& Other)
		: Data(Other.Data)
	{
	}

	FORCEINLINE FString& operator=(const FString& Other)
	{
		Data = Other.Data;
		return *this;
	}

	FORCEINLINE TCHAR& operator[](int32 Index)
	{
		return Data.GetData()[Index];
	}

	FORCEINLINE const TCHAR& operator[](int32 Index) const
	{
		return Data.GetData()[Index];
	}
public:
	FORCEINLINE bool IsEmpty() const
	{
		return Data.Num() <= 1;
	}

	FORCEINLINE const TCHAR* operator*() const
	{
		return Data.Num() ? Data.GetData() : TEXT("");
	}

	FORCEINLINE DataType& GetCharArray()
	{
		return Data;
	}

	FORCEINLINE const DataType& GetCharArray() const
	{
		return Data;
	}

	FString& Append(const TCHAR* Text, int32 Count)
	{
		FString& (*AppendInternal)(FString&, const TCHAR*, int32) = decltype(AppendInternal)(ImageBase + Finder::FindFString_Append());
		return AppendInternal(*this, Text, Count);
	}

	void AppendInt(int32 InNum) {
		void (*AppendIntInternal)(FString&, int32) = decltype(AppendIntInternal)(ImageBase + Finder::FindFString_AppendInt());
		AppendIntInternal(*this, InNum);
	}

	bool ToBool() const {
		bool (*ToBoolInternal)(const FString&) = decltype(ToBoolInternal)(ImageBase + Finder::FindFString_ToBool());
		return ToBoolInternal(*this);
	}

public:
	// Custom
	FString(const wchar_t* Str)
	{
		if (!Str)
			return;

		int32_t Length = (int32_t)wcslen(Str);
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add(Str[i]);

		Data.Add(L'\0');
	}

	FString(const char* Str) {
		if (!Str)
			return;

		int32_t Length = (int32_t)strlen(Str);
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add((wchar_t)Str[i]);

		Data.Add(L'\0');
	}

	FString(const std::string& Str) {
		const wchar_t* WStr = std::wstring(Str.begin(), Str.end()).c_str();
		if (!WStr)
			return;

		int32_t Length = (int32_t)wcslen(WStr);
		Data.Reserve(Length + 1);

		for (int32_t i = 0; i < Length; ++i)
			Data.Add(WStr[i]);

		Data.Add(L'\0');
	}

	std::string ToString() const
	{
		const TCHAR* DataPtr = Data.GetData();

		if (!DataPtr || Data.ArrayNum == 0)
		{
			return std::string();
		}

		size_t Length = std::wcslen(DataPtr);

		if (Length == 0)
		{
			return std::string();
		}

		int size_needed = WideCharToMultiByte(CP_UTF8, 0, DataPtr, (int)Length, NULL, 0, NULL, NULL);
		std::string Result(size_needed, 0);
		WideCharToMultiByte(CP_UTF8, 0, DataPtr, (int)Length, &Result[0], size_needed, NULL, NULL);

		return Result;
	}

	operator std::string() const
	{
		return ToString();
	}

	bool IsValid() const
	{
		return Data.IsValid();
	}
};