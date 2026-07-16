#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "ObjectMacros.h"
#include "Class.h"

class UProperty : public UField {
public:
	DefineCustomProperty(int32, ArrayDim, ServerOffsets::UProperty__ElementSize - 4);

	DefineCustomProperty(int32, ElementSize, ServerOffsets::UProperty__ElementSize);

	DefineCustomProperty(EPropertyFlags, PropertyFlags, ServerOffsets::UProperty__PropertyFlags);

	DefineCustomProperty(int32, Offset_Internal, ServerOffsets::UProperty__Offset_Internal);
public:
	FORCEINLINE int32 GetSize() const { return ElementSize * ArrayDim; }
	FORCEINLINE int32 GetOffset() const { return Offset_Internal; }

	FORCEINLINE bool HasAnyPropertyFlags(EPropertyFlags FlagsToCheck) const { return (PropertyFlags & FlagsToCheck) != 0; }
	FORCEINLINE bool HasAllPropertyFlags(EPropertyFlags FlagsToCheck) const { return (PropertyFlags & FlagsToCheck) == FlagsToCheck; }

	template<typename T>
	FORCEINLINE T* ContainerPtrToValuePtr(void* ContainerPtr, int32 ArrayIndex = 0) const
	{
		return (T*)((uint8*)ContainerPtr + Offset_Internal + (int64)ElementSize * ArrayIndex);
	}

	template<typename T>
	FORCEINLINE const T* ContainerPtrToValuePtr(const void* ContainerPtr, int32 ArrayIndex = 0) const
	{
		return (const T*)((const uint8*)ContainerPtr + Offset_Internal + (int64)ElementSize * ArrayIndex);
	}

	template<typename T>
	FORCEINLINE T& GetPropertyValue(void* ContainerPtr, int32 ArrayIndex = 0) const
	{
		return *ContainerPtrToValuePtr<T>(ContainerPtr, ArrayIndex);
	}

	template<typename T>
	FORCEINLINE void SetPropertyValue(void* ContainerPtr, const T& Value, int32 ArrayIndex = 0) const
	{
		*ContainerPtrToValuePtr<T>(ContainerPtr, ArrayIndex) = Value;
	}

public:
	FORCEINLINE bool IsPODProperty() const { return HasAnyPropertyFlags(CPF_IsPlainOldData); }

	FORCEINLINE void InitializeValue(void* Dest) const
	{
		if (Dest)
			std::memset(Dest, 0, GetSize());
	}

	FORCEINLINE void ClearValue(void* Data) const
	{
		if (Data)
			std::memset(Data, 0, ElementSize);
	}

	FORCEINLINE void CopySingleValue(void* Dest, const void* Src) const
	{
		if (Dest && Src)
			std::memcpy(Dest, Src, ElementSize);
	}

	FORCEINLINE void CopyCompleteValue(void* Dest, const void* Src) const
	{
		if (Dest && Src)
			std::memcpy(Dest, Src, GetSize());
	}

	FORCEINLINE bool Identical(const void* A, const void* B) const
	{
		if (!A || !B)
			return A == B;
		return std::memcmp(A, B, ElementSize) == 0;
	}

	FORCEINLINE void InitializeValue_InContainer(void* Container) const { InitializeValue(ContainerPtrToValuePtr<void>(Container)); }
	FORCEINLINE void ClearValue_InContainer(void* Container, int32 ArrayIndex = 0) const { ClearValue(ContainerPtrToValuePtr<void>(Container, ArrayIndex)); }
	FORCEINLINE void CopyCompleteValue_InContainer(void* DestContainer, const void* SrcContainer) const
	{
		CopyCompleteValue(ContainerPtrToValuePtr<void>(DestContainer), ContainerPtrToValuePtr<void>(SrcContainer));
	}

protected:
	UObject* FindDerivedObjectMember(EClassCastFlags ExpectedFlags, int32 SecondMatch = 0) const
	{
		if (!this)
			return nullptr;

		int32 Matches = 0;

		for (uintptr_t Off = 0x38; Off <= 0xA0; Off += sizeof(void*))
		{
			UObject* Candidate = *reinterpret_cast<UObject**>((uintptr_t)this + Off);
			if (!Candidate)
				continue;

			if (Candidate->IsValidLowLevelFast() && Candidate->IsA(ExpectedFlags))
			{
				if (Matches == SecondMatch)
					return Candidate;
				++Matches;
			}
		}

		return nullptr;
	}

	UProperty* FindChildProperty(int32 WhichMatch = 0) const
	{
		if (!this)
			return nullptr;

		int32 Matches = 0;

		for (uintptr_t Off = 0x38; Off <= 0xA0; Off += sizeof(void*))
		{
			UProperty* Candidate = *reinterpret_cast<UProperty**>((uintptr_t)this + Off);
			if (!Candidate)
				continue;

			if (Candidate->IsValidLowLevelFast() && Candidate->IsA(CASTCLASS_FProperty) && Candidate->GetOuter() == (UObject*)this)
			{
				if (Matches == WhichMatch)
					return Candidate;
				++Matches;
			}
		}

		return nullptr;
	}
};

class UNumericProperty : public UProperty {
public:

};

class UByteProperty : public UNumericProperty {
public:
	UEnum* GetEnum() const { return (UEnum*)FindDerivedObjectMember(CASTCLASS_UEnum); }
};

class UInt8Property : public UNumericProperty {
public:

};

class UInt16Property : public UNumericProperty {
public:

};

class UIntProperty : public UNumericProperty {
public:

};

class UInt64Property : public UNumericProperty {
public:

};

class UUInt16Property : public UNumericProperty {
public:

};

class UUInt32Property : public UNumericProperty {
public:

};

class UUInt64Property : public UNumericProperty {
public:

};

class UFloatProperty : public UNumericProperty {
public:

};

class UDoubleProperty : public UNumericProperty {
public:

};

class UBoolProperty : public UProperty {
public:
	DefineCustomProperty(uint64, FieldMask, ServerOffsets::UBoolProperty__FieldMask);
};

class UObjectPropertyBase : public UProperty {
public:
	UClass* GetPropertyClass() const { return (UClass*)FindDerivedObjectMember(CASTCLASS_UClass); }
};

class UObjectProperty : public UObjectPropertyBase {
public:

};

class UWeakObjectProperty : public UObjectPropertyBase {
public:

};

class ULazyObjectProperty : public UObjectPropertyBase {
public:

};

class UAssetObjectProperty : public UObjectPropertyBase {
public:

};

class UClassProperty : public UObjectProperty {
public:
	UClass* GetMetaClass() const { return (UClass*)FindDerivedObjectMember(CASTCLASS_UClass, 1); }
};

class UAssetClassProperty : public UAssetObjectProperty {
public:

};

class UInterfaceProperty : public UProperty {
public:
	UClass* GetInterfaceClass() const { return (UClass*)FindDerivedObjectMember(CASTCLASS_UClass); }
};

class UNameProperty : public UProperty {
public:

};

class UStrProperty : public UProperty {
public:

};

class UArrayProperty : public UProperty {
public:
	UProperty* GetInner() const { return FindChildProperty(0); }
};

class UMapProperty : public UProperty {
public:
	UProperty* GetKeyProp() const { return FindChildProperty(0); }
	UProperty* GetValueProp() const { return FindChildProperty(1); }
};

class USetProperty : public UProperty {
public:
	UProperty* GetElementProp() const { return FindChildProperty(0); }
};

class UStructProperty : public UProperty {
public:
	UScriptStruct* GetStruct() const { return (UScriptStruct*)FindDerivedObjectMember(CASTCLASS_UScriptStruct); }
};

class UDelegateProperty : public UProperty {
public:

};

class UMulticastDelegateProperty : public UDelegateProperty {
public:

};

class UEnumProperty : public UProperty {
public:
	UEnum* GetEnum() const { return (UEnum*)FindDerivedObjectMember(CASTCLASS_UEnum); }

	UProperty* GetUnderlyingProp() const { return FindChildProperty(0); }
};

using FField = UField;
using FProperty = UProperty;
using FNumericProperty = UNumericProperty;
using FByteProperty = UByteProperty;
using FInt8Property = UInt8Property;
using FInt16Property = UInt16Property;
using FIntProperty = UIntProperty;
using FInt64Property = UInt64Property;
using FUInt16Property = UUInt16Property;
using FUInt32Property = UUInt32Property;
using FUInt64Property = UUInt64Property;
using FFloatProperty = UFloatProperty;
using FDoubleProperty = UDoubleProperty;
using FBoolProperty = UBoolProperty;
using FObjectPropertyBase = UObjectPropertyBase;
using FObjectProperty = UObjectProperty;
using FWeakObjectProperty = UWeakObjectProperty;
using FLazyObjectProperty = ULazyObjectProperty;
using FSoftObjectProperty = UAssetObjectProperty;
using FClassProperty = UClassProperty;
using FSoftClassProperty = UAssetClassProperty;
using FInterfaceProperty = UInterfaceProperty;
using FNameProperty = UNameProperty;
using FStrProperty = UStrProperty;
using FArrayProperty = UArrayProperty;
using FMapProperty = UMapProperty;
using FSetProperty = USetProperty;
using FStructProperty = UStructProperty;
using FDelegateProperty = UDelegateProperty;
using FMulticastDelegateProperty = UMulticastDelegateProperty;
using FEnumProperty = UEnumProperty;

template<typename T> struct TPropertyCastFlags { static constexpr EClassCastFlags Value = CASTCLASS_None; };
template<> struct TPropertyCastFlags<UProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FProperty; };
template<> struct TPropertyCastFlags<UNumericProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FNumericProperty; };
template<> struct TPropertyCastFlags<UByteProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FByteProperty; };
template<> struct TPropertyCastFlags<UInt8Property> { static constexpr EClassCastFlags Value = CASTCLASS_FInt8Property; };
template<> struct TPropertyCastFlags<UInt16Property> { static constexpr EClassCastFlags Value = CASTCLASS_FInt16Property; };
template<> struct TPropertyCastFlags<UIntProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FIntProperty; };
template<> struct TPropertyCastFlags<UInt64Property> { static constexpr EClassCastFlags Value = CASTCLASS_FInt64Property; };
template<> struct TPropertyCastFlags<UUInt16Property> { static constexpr EClassCastFlags Value = CASTCLASS_FUInt16Property; };
template<> struct TPropertyCastFlags<UUInt32Property> { static constexpr EClassCastFlags Value = CASTCLASS_FUInt32Property; };
template<> struct TPropertyCastFlags<UUInt64Property> { static constexpr EClassCastFlags Value = CASTCLASS_FUInt64Property; };
template<> struct TPropertyCastFlags<UFloatProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FFloatProperty; };
template<> struct TPropertyCastFlags<UDoubleProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FDoubleProperty; };
template<> struct TPropertyCastFlags<UBoolProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FBoolProperty; };
template<> struct TPropertyCastFlags<UObjectPropertyBase> { static constexpr EClassCastFlags Value = CASTCLASS_FObjectPropertyBase; };
template<> struct TPropertyCastFlags<UObjectProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FObjectProperty; };
template<> struct TPropertyCastFlags<UWeakObjectProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FWeakObjectProperty; };
template<> struct TPropertyCastFlags<ULazyObjectProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FLazyObjectProperty; };
template<> struct TPropertyCastFlags<UAssetObjectProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FSoftObjectProperty; };
template<> struct TPropertyCastFlags<UClassProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FClassProperty; };
template<> struct TPropertyCastFlags<UAssetClassProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FSoftClassProperty; };
template<> struct TPropertyCastFlags<UInterfaceProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FInterfaceProperty; };
template<> struct TPropertyCastFlags<UNameProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FNameProperty; };
template<> struct TPropertyCastFlags<UStrProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FStrProperty; };
template<> struct TPropertyCastFlags<UArrayProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FArrayProperty; };
template<> struct TPropertyCastFlags<UMapProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FMapProperty; };
template<> struct TPropertyCastFlags<USetProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FSetProperty; };
template<> struct TPropertyCastFlags<UStructProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FStructProperty; };
template<> struct TPropertyCastFlags<UDelegateProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FDelegateProperty; };
template<> struct TPropertyCastFlags<UMulticastDelegateProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FMulticastDelegateProperty; };
template<> struct TPropertyCastFlags<UEnumProperty> { static constexpr EClassCastFlags Value = CASTCLASS_FEnumProperty; };

template<typename To>
FORCEINLINE To* CastField(UField* Field)
{
	return (Field && Field->IsA(TPropertyCastFlags<To>::Value)) ? (To*)Field : nullptr;
}

template<typename To>
FORCEINLINE const To* CastField(const UField* Field)
{
	return (Field && Field->IsA(TPropertyCastFlags<To>::Value)) ? (const To*)Field : nullptr;
}

#define DefineUProperty(Type, Name) \
private: \
    static inline int32 Name##_Offset = -2; \
public: \
    bool _Has##Name() const { \
        if (Name##_Offset == -2) { \
            UClass* Class = StaticClass(); \
            if (Class) Name##_Offset = (int32)Class->GetPropertyOffset(#Name); \
        } \
        return Name##_Offset >= 0; \
    } \
    Type& _Get##Name() { \
        if (Name##_Offset == -2) { \
            UClass* Class = StaticClass(); \
            if (Class) Name##_Offset = (int32)Class->GetPropertyOffset(#Name); \
        } \
        if (Name##_Offset < 0) { \
            static Type dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    Type _Get##Name() const { \
        if (Name##_Offset == -2) { \
            UClass* Class = StaticClass(); \
            if (Class) Name##_Offset = (int32)Class->GetPropertyOffset(#Name); \
        } \
        if (Name##_Offset < 0) { \
            static Type dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    void _Set##Name(Type Value) { \
        if (Name##_Offset == -2) { \
            UClass* Class = StaticClass(); \
            if (Class) Name##_Offset = (int32)Class->GetPropertyOffset(#Name); \
        } \
        if (Name##_Offset < 0) { \
            return; \
        } \
        *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset) = Value; \
    } \
    __declspec(property(get=_Get##Name, put=_Set##Name)) Type Name;

#define DefineStructProperty(Type, Name) \
private: \
    static inline int32 Name##_Offset = -2; \
public: \
    Type& _Get##Name() { \
        if (Name##_Offset == -2) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
        } \
        if (Name##_Offset < 0) { \
            static Type dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    Type& _Get##Name() const { \
        if (Name##_Offset == -2) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
        } \
        if (Name##_Offset < 0) { \
            static Type dummy{}; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    void _Set##Name(Type Value) { \
        if (Name##_Offset == -2) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
        } \
        if (Name##_Offset < 0) { \
            return; \
        } \
        *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset) = Value; \
    } \
    __declspec(property(get=_Get##Name, put=_Set##Name)) Type Name;

#define DefineBitfieldUProperty(Name) \
private: \
    static inline int32 Name##_Offset = -1; \
    static inline uint8_t Name##_FieldMask = 0; \
public: \
    bool _Get##Name() { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)this->FindPropertyByName(#Name); \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return false; \
        } \
        uint8_t value = *reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        return (value & Name##_FieldMask) != 0; \
    } \
    bool _Get##Name() const { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)this->FindPropertyByName(#Name); \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return false; \
        } \
        uint8_t value = *reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        return (value & Name##_FieldMask) != 0; \
    } \
    void _Set##Name(bool Value) { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)this->FindPropertyByName(#Name); \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
                return; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return; \
        } \
        uint8_t* ptr = reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        if (Value) { \
            *ptr |= Name##_FieldMask; \
        } else { \
            *ptr &= ~Name##_FieldMask; \
        } \
    } \
    __declspec(property(get=_Get##Name, put=_Set##Name)) bool Name;

#define DefineBitfieldStructProperty(Name) \
private: \
    static inline int32 Name##_Offset = -1; \
    static inline uint8_t Name##_FieldMask = 0; \
public: \
    bool _Get##Name() { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)StaticStruct()->FindPropertyByName(#Name);       \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return false; \
        } \
        uint8_t value = *reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        return (value & Name##_FieldMask) != 0; \
    } \
    bool _Get##Name() const { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)StaticStruct()->FindPropertyByName(#Name); \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return false; \
        } \
        uint8_t value = *reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        return (value & Name##_FieldMask) != 0; \
    } \
    void _Set##Name(bool Value) { \
        if (Name##_Offset == -1) { \
            UBoolProperty* Prop = (UBoolProperty*)StaticStruct()->FindPropertyByName(#Name); \
            if (Prop) { \
                Name##_Offset = Prop->Offset_Internal; \
                Name##_FieldMask = Prop->FieldMask; \
            } else { \
                Name##_Offset = -1; \
                return; \
            } \
        } \
        if (Name##_Offset < 0) { \
            return; \
        } \
        uint8_t* ptr = reinterpret_cast<uint8_t*>((uintptr_t)this + Name##_Offset); \
        if (Value) { \
            *ptr |= Name##_FieldMask; \
        } else { \
            *ptr &= ~Name##_FieldMask; \
        } \
    } \
    __declspec(property(get=_Get##Name, put=_Set##Name)) bool Name;

#define DefineEnumProperty(Name) \
private: \
    static inline bool Name##__Initialized = false; \
    static inline uint8 Name##__Value = 0; \
public: \
    static uint8 Get##Name() \
    { \
        if (!Name##__Initialized) \
        { \
            UEnum* Enum = StaticEnum(); \
            \
            if (Enum) \
            { \
                Name##__Value = \
                    static_cast<uint8>(Enum->GetValue(#Name)); \
            } \
            \
            Name##__Initialized = true; \
        } \
        \
        return Name##__Value; \
    }