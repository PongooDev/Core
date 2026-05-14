#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/CoreUObject/Public/UObject/PersistentObjectPtr.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/AssetPtr.h"

class FSoftObjectPtr : public TPersistentObjectPtr<FSoftObjectPath>
{
public:
    const UObject* InternalGet(const UClass* Class)
    {
        if (!this)
            return nullptr;

        auto Object = WeakPtr.Get();

        if (!Object)
        {
            const UObject* Ret = nullptr;

            if (Version::Engine_Version <= 4.16)
            {
                auto AssetLongPathname = *(FString*)(__int64(this) + offsetof(FSoftObjectPtr, ObjectID));

                if (!AssetLongPathname.IsEmpty() && AssetLongPathname.IsValid())
                    WeakPtr = Ret = StaticLoadObject(AssetLongPathname);
            }
            if (Version::Fortnite_Version >= 23)
            {
                auto& PackageName = *(FName*)(__int64(this) + (Version::Engine_Version < 5.3 ? 0xC : 0x8));
                auto& AssetName = *(FName*)(__int64(this) + (Version::Engine_Version < 5.3 ? 0x10 : 0xC));
                auto& SubPathString = *(FString*)(__int64(this) + (Version::Engine_Version < 5.3 ? 0x14 : 0x10));

                if (PackageName.ComparisonIndex > 0)
                {
                   std::string FullPath = PackageName.ToString().ToString();
                    if (AssetName.ComparisonIndex > 0)
                        FullPath =+ "." + AssetName.ToString().ToString();
                    if (!SubPathString.IsEmpty() && SubPathString.IsValid())
                        FullPath =+ ":" + SubPathString.ToString();
                    WeakPtr = Ret = StaticLoadObject(FullPath);
                }
            }
            else if (ObjectID.AssetPathName.ComparisonIndex > 0)
            {
                auto FullPath = ObjectID.AssetPathName.ToString().ToString();
                if (!ObjectID.SubPathString.IsEmpty() && ObjectID.SubPathString.IsValid())
                    FullPath += ":" + ObjectID.SubPathString.ToString();

                WeakPtr = Ret = StaticLoadObject(FullPath.c_str(), Class);
            }

            return Ret;
        }

        return Object;
    }

    static uint32_t GetSize()
    {
        return Version::Engine_Version >= 5.3 ? 0x20 : sizeof(FSoftObjectPtr);
    }
};

template <typename UEType>
class TSoftObjectPtr : public FSoftObjectPtr
{
public:
    TSoftObjectPtr()
    {
    }

    TSoftObjectPtr(UEType* Obj)
    {
        WeakPtr = FWeakObjectPtr(Obj);
    }

    UEType* Get()
    {
        return (UEType*)InternalGet(UEType::StaticClass());
    }
    const UEType* operator->()
    {
        return Get();
    }
    operator const UEType* ()
    {
        return Get();
    }
};