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
        if (!(this) || (uintptr_t)(Offset) <= 0) { \
            static PropertyType dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + (uintptr_t)(Offset)); \
    } \
    FORCEINLINE PropertyType _Get##PropertyName() const { \
        if (!(this) || (uintptr_t)(Offset) <= 0) { \
            static PropertyType dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<PropertyType*>((uintptr_t)this + (uintptr_t)(Offset)); \
    } \
    FORCEINLINE void _Set##PropertyName(PropertyType Value) { \
        if (!(this) || (uintptr_t)(Offset) <= 0) { \
            return; \
        } \
        *reinterpret_cast<PropertyType*>((uintptr_t)this + (uintptr_t)(Offset)) = Value; \
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
    uintptr_t GetPropertyOffset(std::string InName) const;
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
        if (!SuperStruct) 
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

    uint32 GetVTableIndex() const // this one took from erbium
    {
        if (!this)
            return -1;

        std::string FuncName = GetFName().ToString().ToString();
        std::string ValidateName = std::string(FuncName.c_str()) + "_Validate";
        auto ValidateRef = Memcury::Scanner::FindStringRef(ValidateName.c_str(), false);

        auto Addr = ValidateRef.Get();

        if (!Addr)
            Addr = Memcury::Scanner(Func).ScanFor({ 0x0F, 0x95 }).Get();

        if (Addr)
            for (int i = 0; i < 2000; i++)
            {
                if (*((uint8*)Addr + i) == 0xFF && (*((uint8*)Addr + i + 1) == 0x90 || *((uint8*)Addr + i + 1) == 0x93 || *((uint8*)Addr + i + 1) == 0xA0))
                {
                    auto VTIndex = *(uint32_t*)(Addr + i + 2);

                    return VTIndex / 8;
                }
            }

        return -1;
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
    int64 GetValue(const char* EnumMemberName) const;
};

namespace EIncludeSuperFlag
{
    enum Type
    {
        ExcludeSuper,
        IncludeSuper
    };
}

enum class EGetSparseClassDataMethod : uint8
{
    /**
     * If not yet created, store and return a new instance, whether or not the class can use its archetype's instance.
     */
    CreateIfNull,
    /**
     * If not yet created, return the archetype instance (if possible to use for this class). If not possible to use
     * the archetype's instance, store and return a new instance.
     */
    ArchetypeIfNull,
    /**
     * This is an advanced flag and should usually be avoided. Callers should instead call with ArchetypeIfNull if they
     * are currently allowed to create an instance, or with DeferIfNull if not. Callers that want to know whether the
     * class has sparse data of its own beyond what the archetype has should compare
     * GetSparseClassDataStruct() != GetSparseClassDataArchetypeStruct().
     *
     * If not yet created, return null. An instance might still be created later when called with ArchetypeIfNull.
     * This call never creates an instance.
     */
    ReturnIfNull,
    /**
     * If not yet created, return the archetype's instance (if possible to use for this class). If not possible to use
     * the archetype's instance, return null. User should call later with ArchetypeIfNull when they are allowed to
     * create an instance.
     * This call never creates an instance.
     */
    DeferIfNull,
};

class UClass : public UStruct {
public:
    UFunction* FindFunctionByName(FName InName, EIncludeSuperFlag::Type IncludeSuper = EIncludeSuperFlag::IncludeSuper) const;

    UFunction* GetFunction(const std::string& ClassName, const std::string& FuncName) const;

    UFunction* GetFunction(const std::string& FullName, bool bSilent = false) const;

    const void* GetSparseClassData(const EGetSparseClassDataMethod GetMethod);
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

template <typename Ret, typename... Args>
Ret UObject::Call(UFunction* Function, Args&&... args)
{

    if (!Function)
        return Ret();

    // fast paths
    if constexpr (sizeof...(args) == 0 && std::is_void_v<Ret>)
        return ProcessEvent(Function, nullptr);

    if constexpr (sizeof...(args) == 1 && std::is_void_v<Ret>)
        return ProcessEvent(Function, (void*)&args...);

    if constexpr (sizeof...(args) == 0 && !std::is_void_v<Ret>)
    {
        Ret ret{};

        ProcessEvent(Function, &ret);

        return ret;
    }

    auto Params = Function->GetParams();
    auto Mem = FMemory::Malloc(Params.Size);
    memset((PBYTE)Mem, 0, Params.Size);

    size_t i = 0;
    ([&]
        {
            if (i >= Params.NameOffsetMap.size())
                return;

            auto& Param = Params.NameOffsetMap[i];

            if (((Param.PropertyFlags & 0x100) != 0 && (Param.PropertyFlags & 0x8000000) == 0) || (Param.PropertyFlags & 0x400) != 0)
            {
                i++;
                return;
            }

            const auto& Arg = args;

            memcpy(PBYTE(__int64(Mem) + Param.Offset), (const PBYTE)&Arg, Param.ElementSize);
            i++;
        }(), ...);

    ProcessEvent(Function, Mem);

    i = 0;
    ([&]
        {
            if (i >= Params.NameOffsetMap.size())
                return;

            auto& Param = Params.NameOffsetMap[i];

            if (((Param.PropertyFlags & 0x100) == 0 && (Param.PropertyFlags & 0x8000000) == 0) || (Param.PropertyFlags & 0x400) != 0)
            {
                i++;
                return;
            }

            const auto& Arg = args;

            if constexpr (std::is_pointer_v<std::remove_reference_t<decltype(args)>>)
            {
                if (Arg != nullptr)
                    memcpy((PBYTE)Arg, (const PBYTE)(__int64(Mem) + Param.Offset), Param.ElementSize);
            }
            else if constexpr (std::is_reference_v<decltype(args)>)
            {
                // if ((Param.PropertyFlags & 0x2) != 0)
                //	memcpy((PBYTE)&Arg, (const PBYTE)(__int64(Mem) + Param.Offset), Param.ElementSize);
            }
            i++;
        }(), ...);

    if constexpr (!std::is_void_v<Ret>)
    {
        Ret ret{};
        for (auto& Param : Params.NameOffsetMap)
        {
            if ((Param.PropertyFlags & 0x400) == 0)
                continue;

            memcpy((PBYTE)&ret, (const PBYTE)(__int64(Mem) + Param.Offset), Param.ElementSize);
            break;
        }

        FMemory::Free(Mem);
        return ret;
    }

    FMemory::Free(Mem);
}

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
    } \
    static __Class* Allocate() \
    { \
        void* Mem = FMemory::Malloc(GetSize()); \
        if (Mem) memset(Mem, 0, GetSize()); \
        return (__Class*)Mem; \
    } \
    static void Copy(__Class* Dest, const __Class* Src) \
    { \
        memcpy(Dest, Src, GetSize()); \
    } \

#define DefineUnrealStructPath(FullName) \
    static UStruct* StaticStruct() \
    { \
        static UStruct* CachedStaticStruct = nullptr; \
        static bool bInitialized = false; \
        \
        if (!bInitialized) \
        { \
            bInitialized = true; \
            CachedStaticStruct = (UStruct*)FUObjectArray::FindObject(FullName, true); \
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
                Log("Failed to find size for " #FullName "!"); \
                return 0; \
            } \
        } \
        return Size; \
    } \
    static void* Allocate() \
    { \
        void* Mem = FMemory::Malloc(GetSize()); \
        if (Mem) memset(Mem, 0, GetSize()); \
        return Mem; \
    } \

#define DefineUnrealEnum(__Class) \
    static UEnum* StaticEnum() \
    { \
        static UEnum* CachedStaticEnum = nullptr; \
        static bool bInitialized = false; \
        \
        if (!bInitialized) \
        { \
            bInitialized = true; \
            CachedStaticEnum = (UEnum*)FUObjectArray::FindObjectFast(#__Class);  \
        } \
        return CachedStaticEnum; \
    }