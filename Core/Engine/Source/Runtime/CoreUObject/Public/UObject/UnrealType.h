#pragma once
#include "pch.h"

#include "Engine/Source/Runtime/Core/Public/HAL/Platform.h"
#include "ObjectMacros.h"
#include "Class.h"

class UProperty : public UField {
public:
	DefineCustomProperty(int32, ElementSize, ServerOffsets::UProperty__ElementSize);

	DefineCustomProperty(EPropertyFlags, PropertyFlags, ServerOffsets::UProperty__PropertyFlags);

	DefineCustomProperty(int32, Offset_Internal, ServerOffsets::UProperty__Offset_Internal);
};

class UNumericProperty : public UProperty {
public:

};

class UByteProperty : public UNumericProperty {
public:

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

};

class UAssetClassProperty : public UAssetObjectProperty {
public:

};

class UInterfaceProperty : public UProperty {
public:

};

class UNameProperty : public UProperty {
public:

};

class UStrProperty : public UProperty {
public:

};

class UArrayProperty : public UProperty {
public:

};

class UMapProperty : public UProperty {
public:

};

class USetProperty : public UProperty {
public:

};

class UStructProperty : public UProperty {
public:

};

class UDelegateProperty : public UProperty {
public:

};

class UMulticastDelegateProperty : public UDelegateProperty {
public:

};

#define DefineUProperty(Type, Name) \
private: \
    static inline int32 Name##_Offset = -1; \
public: \
    Type& _Get##Name() { \
        if (Name##_Offset == -1) { \
            Name##_Offset = StaticClass()->GetPropertyOffset(#Name); \
            if (Name##_Offset == -1) { \
                Log("Failed to find property: " #Name); \
            } \
        } \
        if (Name##_Offset <= 0) { \
            static Type dummy; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    Type _Get##Name() const { \
        if (Name##_Offset == -1) { \
            Name##_Offset = StaticClass()->GetPropertyOffset(#Name); \
            if (Name##_Offset == -1) { \
                Log("Failed to find property: " #Name); \
            } \
        } \
        if (Name##_Offset <= 0) { \
            static Type dummy; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    void _Set##Name(Type Value) { \
        if (Name##_Offset == -1) { \
            Name##_Offset = StaticClass()->GetPropertyOffset(#Name); \
            if (Name##_Offset == -1) { \
                Log("Failed to find property: " #Name); \
                return; \
            } \
        } \
        if (Name##_Offset <= 0) { \
            return; \
        } \
        *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset) = Value; \
    } \
    __declspec(property(get=_Get##Name, put=_Set##Name)) Type Name;

#define DefineStructProperty(Type, Name) \
private: \
    static inline int32 Name##_Offset = -1; \
public: \
    Type& _Get##Name() { \
        if (Name##_Offset == -1) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
            if (Name##_Offset == -1) { \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
            static Type dummy; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    Type& _Get##Name() const { \
        if (Name##_Offset == -1) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
            if (Name##_Offset == -1) { \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
            static Type dummy; \
            return dummy; \
        } \
        return *reinterpret_cast<Type*>((uintptr_t)this + Name##_Offset); \
    } \
    void _Set##Name(Type Value) { \
        if (Name##_Offset == -1) { \
            UStruct* Struct = StaticStruct(); \
            if (Struct) { \
                Name##_Offset = Struct->GetPropertyOffset(#Name); \
            } \
            if (Name##_Offset == -1) { \
                return; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
                return; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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
                Log("Failed to find bitfield property: " #Name); \
                Name##_Offset = 0; \
                return; \
            } \
        } \
        if (Name##_Offset <= 0) { \
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