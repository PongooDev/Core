#include "pch.h"
#include "Engine/Source/Runtime/Core/Public/Logging/LogMacros.h"

#include "Engine/Source/Runtime/Engine/Classes/Kismet/KismetStringLibrary.h"

const FLogCategoryName& FLogCategoryBase::GetCategoryName() const
{
    if (!bResolvedName)
    {
        bResolvedName = true;
        CategoryFName = UKismetStringLibrary::Conv_StringToName(CategoryNameString);
    }

    return CategoryFName;
}

void* FMsg::GetLogfAddress()
{
    static void* Addr = nullptr;

    if (!Addr)
    {
        const uintptr_t Offset = Finder::FindFMsg_Logf();
        if (Offset)
            Addr = (void*)(ImageBase + Offset);
    }

    return Addr;
}