#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/Object.h"
#include "Engine/Source/Runtime/CoreUObject/Public/UObject/UObjectArray.h"

class UStruct;
class FField;
class UProperty;
class UClass;
struct FCustomPropertyListNode;
struct FFrame;
struct FNetDeltaSerializeInfo;
struct FObjectInstancingGraph;
struct FPropertyTag;

#define DefineCustomProperty(PropertyType, PropertyName, Offset) \
public: \
    FORCEINLINE PropertyType& _Get##PropertyName() { \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset); \
    } \
    FORCEINLINE PropertyType _Get##PropertyName() const { \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset); \
    } \
    FORCEINLINE void _Set##PropertyName(PropertyType Value) { \
        *reinterpret_cast<PropertyType*>((uintptr_t)this + Offset) = Value; \
    } \
public: \
    __declspec(property(get = _Get##PropertyName, put = _Set##PropertyName)) PropertyType PropertyName;


class UField : public UObject {
public:
    DefineCustomProperty(UField*, Next, ServerOffsets::UField__Next);

    DefineCustomProperty(FName, Name, ServerOffsets::UField__Name);
public:
    UClass* GetOwnerClass() const;

    UStruct* GetOwnerStruct() const;
};

class UStruct : public UField {
public:
    DefineCustomProperty(UStruct*, SuperStruct, ServerOffsets::UStruct__SuperStruct);

    DefineCustomProperty(UField*, Children, ServerOffsets::UStruct__Children);

    DefineCustomProperty(int32, PropertiesSize, ServerOffsets::UStruct__PropertiesSize);

    DefineCustomProperty(int32, MinAlignment, ServerOffsets::UStruct__MinAlignment);

    DefineCustomProperty(UField*, ChildProperties, 0x50);
public:
    UProperty* FindPropertyByName(FName InName) const;
    UProperty* FindPropertyByName(std::string InName) const;
    uintptr_t GetPropertyOffset(std::string InName) const;

    bool IsChildOf(const UStruct* SomeBase) const;
public:
    UField* GetChildrenOrChildProperties() const
    {
        if (Version::Fortnite_Version >= 12.10)
            return ChildProperties;
        else
            return Children;
    }

	bool IsSubclassOf(const UStruct* Base) const
	{
		if (!Base)
			return false;

		for (const UStruct* Struct = this; Struct; Struct = Struct->SuperStruct)
		{
			if (Struct == Base)
				return true;
		}

		return false;
	}
};

class UScriptStruct : public UStruct {
public:

};

class UFunction : public UStruct {
public:
    DefineCustomProperty(void*, Func, ServerOffsets::ExecFunction);
public:
    struct Param
    {
        FString Name;
        uint32 Offset;
        uint64 PropertyFlags;
        uint32 ElementSize;
    };

    class Params
    {
    public:
        std::vector<Param> NameOffsetMap;
        uint32 Size;
    };

    Params GetParams() const
    {
        Params p{};

        for (const UField* _Pr = GetChildrenOrChildProperties(); _Pr; _Pr = _Pr->Next)
            p.NameOffsetMap.push_back({ _Pr->Name.ToString(), GetFromOffset<uint32>(_Pr, ServerOffsets::UProperty__Offset_Internal),
                GetFromOffset<uint64>(_Pr, ServerOffsets::UProperty__PropertyFlags), GetFromOffset<uint32>(_Pr, ServerOffsets::UProperty__ElementSize) });

        p.Size = PropertiesSize;
        return p;
    }
public:
    static UClass* StaticClass() {
        return (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Function");
    }
};

class UDelegateFunction : public UFunction {
public:

};

class UEnum : public UField {
public:

};

namespace EIncludeSuperFlag
{
    enum Type
    {
        ExcludeSuper,
        IncludeSuper
    };
}

class UClass : public UStruct {
public:
    UFunction* FindFunctionByName(FName InName, EIncludeSuperFlag::Type IncludeSuper = EIncludeSuperFlag::IncludeSuper) const;

    UFunction* GetFunction(const std::string& ClassName, const std::string& FuncName) const;

    UFunction* GetFunction(const std::string& FullName, bool bSilent = false) const;
public:
	inline uint64_t GetCastFlags() const
	{
		static int32 Offset = 0;
		if (Offset == 0)
		{
			auto ClassClass = (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Class");
			auto ActorClass = (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Object");
			auto ClassObj = FUObjectArray::DefaultObjImpl(ClassClass);
			auto ActorObj = FUObjectArray::DefaultObjImpl(ActorClass);
			for (int i = 0x28; i < 0x1a0; i += 4)
			{
				if (*(uint64_t*)(__int64(ClassObj) + i) == 0x29 && *(uint64_t*)(__int64(ActorObj) + i) == 0x1000000000)
				{
					Offset = i;
					break;
				}
			}
		}

		return *(uint64_t*)(__int64(this) + Offset);
	}

	inline UObject* GetDefaultObject() const
	{
		if (!this)
			return nullptr;

		static int32 Offset = 0;
		if (Offset == 0)
		{
			auto ClassClass = (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Class");
			auto ActorClass = (UClass*)FUObjectArray::FindObject("Class /Script/CoreUObject.Object");
			auto ClassObj = FUObjectArray::DefaultObjImpl(ClassClass);
			auto ActorObj = FUObjectArray::DefaultObjImpl(ActorClass);
			for (int i = 0x28; i < 0x1a0; i += 4)
			{
				if (*(UObject**)(__int64(ClassClass) + i) == ClassObj && *(UObject**)(__int64(ActorClass) + i) == ActorObj)
				{
					Offset = i;
					break;
				}
			}
		}

		return *(UObject**)(__int64(this) + Offset);
	}

	inline UObject* GetDefaultObj() const
	{
		return GetDefaultObject();
	}
};

class UDynamicClass : public UClass {
public:

};

#define DefineUnrealClass(__Class) \
    static UClass* StaticClass() \
    { \
        static UClass* CachedStaticClass = nullptr; \
        static bool bInitialized = false; \
        if (!bInitialized) \
        { \
            bInitialized = true; \
            CachedStaticClass = (UClass*)FUObjectArray::FindObjectFast(#__Class + 1); \
        } \
        return CachedStaticClass; \
    } \
    \
    static __Class* GetDefaultObj() \
    { \
        static UObject* CachedDefaultObject = nullptr; \
        static bool bInitialized = false; \
        \
        if (!bInitialized) \
        { \
            bInitialized = true; \
            CachedDefaultObject = StaticClass()->GetDefaultObj(); \
        } \
        \
        return (__Class*)CachedDefaultObject; \
    } \
    \
    static int32 GetSize() \
    { \
        static int32 Size = -1; \
        if (Size == -1) \
        { \
            Size = StaticClass()->PropertiesSize; \
            if (Size <= 0) \
            { \
                Log("Failed to find size for " #__Class "!"); \
                return 0; \
            } \
        } \
        return Size; \
    }

#define DefineUnrealStruct(__Class) \
    static UStruct* StaticStruct() \
    { \
        static UStruct* CachedStaticStruct = nullptr; \
        static bool bInitialized = false; \
        \
        if (!bInitialized) \
        { \
            bInitialized = true; \
            CachedStaticStruct = (UStruct*)FUObjectArray::FindObjectFast(#__Class + 1);  \
        } \
        return CachedStaticStruct; \
    } \
    \
    static int32 GetSize() \
    { \
        static int32 Size = -1; \
        if (Size == -1) \
        { \
            Size = StaticStruct()->PropertiesSize; \
            if (Size <= 0) \
            { \
                Log("Failed to find size for " #__Class "!"); \
                return 0; \
            } \
        } \
        return Size; \
    }