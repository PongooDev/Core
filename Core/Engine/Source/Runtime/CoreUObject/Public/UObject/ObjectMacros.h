#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"

enum EClassCastFlags : uint64
{
	RF_NoFlags = 0x00000000,

	CASTCLASS_UClass = 0x0000000000000020,
	CASTCLASS_UFunction = 0x0000000000080000,
};

enum EPropertyFlags : uint64
{

};

enum EObjectFlags
{
	RF_HasExternalPackage = 0x10000000,

	RF_ClassDefaultObject = 0x00000010,
};

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