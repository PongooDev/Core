#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Misc/AssertionMacros.h"

#include "Engine/Source/Runtime/Core/Public/Containers/UnrealString.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"
#include "Engine/Source/Runtime/Core/Public/CoreGlobals.h"

void FDebug::AssertFailed(const char* Expr, const char* File, int32 Line, const TCHAR* Format, ...)
{
    void (*AssertFailedInternal)(const char* Expr, const char* File, int32 Line, const TCHAR * Format, va_list Args)
        = decltype(AssertFailedInternal)(ImageBase + Finder::FindFDebug_AssertFailed());

    va_list args;
    va_start(args, Format);

    AssertFailedInternal(Expr, File, Line, Format, args);

    va_end(args);
}

void FDebug::LogAssertFailedMessage(const char* Expr, const char* File, int32 Line, const TCHAR* Format, ...)
{
    TCHAR DescriptionString[2048];
    DescriptionString[0] = TEXT('\0');

    if (Format && Format[0] != L'\0')
    {
        va_list args;
        va_start(args, Format);

        vswprintf(DescriptionString, 2048, Format, args);

        va_end(args);
    }

    UE_LOG(LogOutputDevice, Error, TEXT("Assertion failed: %s [File:%s] [Line: %d] %s"),
        *FString(Expr), *FString(File), Line, DescriptionString);
}