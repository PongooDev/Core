#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/UObject/NameTypes.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogVerbosity.h"

using FLogCategoryName = FName;

struct FMsg
{
public:
    static void* GetLogfAddress();

    template <typename... Types>
    static void Logf_Internal(const char* File, int32 Line, const FLogCategoryName& Category, ELogVerbosity::Type Verbosity, const TCHAR* Fmt, Types... Args)
    {
        using FLogfPtr = void (*)(const char*, int32, const FLogCategoryName&, ELogVerbosity::Type, const TCHAR*, ...);

        if (FLogfPtr Logf = (FLogfPtr)GetLogfAddress())
            Logf(File, Line, Category, Verbosity, Fmt, Args...);
    }
};

class FLogCategoryBase
{
public:
    FLogCategoryBase(const TCHAR* InCategoryName, ELogVerbosity::Type InDefaultVerbosity)
        : CategoryNameString(InCategoryName)
        , CategoryFName{}
        , bResolvedName(false)
        , Verbosity(InDefaultVerbosity)
        , DefaultVerbosity(InDefaultVerbosity)
    {
    }

    FORCEINLINE bool IsSuppressed(ELogVerbosity::Type VerbosityLevel) const
    {
        return (VerbosityLevel & ELogVerbosity::VerbosityMask) > Verbosity;
    }

    FORCEINLINE ELogVerbosity::Type GetVerbosity() const { return Verbosity; }

    void SetVerbosity(ELogVerbosity::Type NewVerbosity)
    {
        Verbosity = ELogVerbosity::Type(NewVerbosity & ELogVerbosity::VerbosityMask);
    }

    void ResetFromDefault() { Verbosity = DefaultVerbosity; }

    const FLogCategoryName& GetCategoryName() const;

private:
    const TCHAR* CategoryNameString;

    mutable FLogCategoryName CategoryFName;
    mutable bool bResolvedName;

    ELogVerbosity::Type Verbosity;
    const ELogVerbosity::Type DefaultVerbosity;
};

template <ELogVerbosity::Type InDefaultVerbosity, ELogVerbosity::Type InCompileTimeVerbosity>
class FLogCategory : public FLogCategoryBase
{
public:
    static constexpr ELogVerbosity::Type CompileTimeVerbosity = InCompileTimeVerbosity;

    FLogCategory(const TCHAR* InCategoryName)
        : FLogCategoryBase(InCategoryName, InDefaultVerbosity)
    {
    }
};

#ifndef COMPILED_IN_MINIMUM_VERBOSITY
    #define COMPILED_IN_MINIMUM_VERBOSITY All
#endif

#define DECLARE_LOG_CATEGORY_EXTERN(CategoryName, DefaultVerbosity, CompileTimeVerbosity) \
    extern class FLogCategory##CategoryName : public FLogCategory<ELogVerbosity::DefaultVerbosity, ELogVerbosity::CompileTimeVerbosity> \
    { \
    public: \
        FORCEINLINE FLogCategory##CategoryName() : FLogCategory(TEXT(#CategoryName)) {} \
    } CategoryName;

#define DEFINE_LOG_CATEGORY(CategoryName) FLogCategory##CategoryName CategoryName;

#define DECLARE_LOG_CATEGORY_STATIC(CategoryName, DefaultVerbosity, CompileTimeVerbosity) \
    static class FLogCategory##CategoryName : public FLogCategory<ELogVerbosity::DefaultVerbosity, ELogVerbosity::CompileTimeVerbosity> \
    { \
    public: \
        FORCEINLINE FLogCategory##CategoryName() : FLogCategory(TEXT(#CategoryName)) {} \
    } CategoryName;

#define UE_LOG(CategoryName, Verbosity, Format, ...) \
    { \
        static_assert((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) > 0 \
            && (ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) < ELogVerbosity::NumVerbosity, \
            "Verbosity must be constant and in range."); \
        if constexpr ((ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) <= ELogVerbosity::COMPILED_IN_MINIMUM_VERBOSITY \
            && (ELogVerbosity::Verbosity & ELogVerbosity::VerbosityMask) <= std::remove_reference_t<decltype(CategoryName)>::CompileTimeVerbosity) \
        { \
            if (!(CategoryName).IsSuppressed(ELogVerbosity::Verbosity)) \
            { \
                FMsg::Logf_Internal(__FILE__, __LINE__, (CategoryName).GetCategoryName(), ELogVerbosity::Verbosity, Format, ##__VA_ARGS__); \
            } \
        } \
    }

#define UE_CLOG(Condition, CategoryName, Verbosity, Format, ...) \
    { \
        if (Condition) \
        { \
            UE_LOG(CategoryName, Verbosity, Format, ##__VA_ARGS__); \
        } \
    }
