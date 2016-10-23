//
// Copyright (c) 2008-2016 the Urho3D project.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "RefCounted.h"
#include "Ptr.h"
#include "CCPlatformDefine.h"
#include <string>
#include <map>
#include "ID.h"


namespace cocos2d
{

enum VariantType
{
    VAR_NONE = 0,
    VAR_INT,
    VAR_BOOL,
    VAR_FLOAT,
	VAR_DOUBLE,
	VAR_STRING,

	VAR_PTR,
	VAR_VOIDPTR,

    MAX_VAR_TYPES
};

/// Union for the possible variant values. Also stores non-POD objects such as String and math objects (excluding Matrix) which must not exceed 16 bytes in size. Objects exceeding 16 bytes size are stored in the heap pointed by _ptr.
struct VariantValue
{
    union
    {
        int int_;
        bool bool_;
        float float_;
        void* ptr_;
    };

    union
    {
        int int2_;
        float float2_;
        void* ptr2_;
    };

    union
    {
        int int3_;
        float float3_;
        void* ptr3_;
    };

    union
    {
        int int4_;
        float float4_;
        void* ptr4_;
    };
};

class Variant;

typedef std::map<ID, Variant> VariantMap;

/// Variable that supports a fixed set of types.
class CC_DLL Variant
{
public:
    /// Construct empty.
    Variant() :
        type_(VAR_NONE)
    {
    }

    /// Construct from integer.
    Variant(int value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from unsigned integer.
    Variant(unsigned value) :
        type_(VAR_NONE)
    {
        *this = (int)value;
    }

    /// Construct from a bool.
    Variant(bool value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a float.
    Variant(float value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a double.
    Variant(double value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a string.
    Variant(const std::string& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a C string.
    Variant(const char* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

	/// Construct from a pointer.
    Variant(void* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from a RefCounted pointer. The object will be stored internally in a WeakPtr so that its expiration can be detected safely.
    Variant(RefCounted* value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Construct from type and value.
    Variant(const std::string& type, const std::string& value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(VariantType type, const std::string& value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(const char* type, const char* value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Construct from type and value.
    Variant(VariantType type, const char* value) :
        type_(VAR_NONE)
    {
        FromString(type, value);
    }

    /// Copy-construct from another variant.
    Variant(const Variant& value) :
        type_(VAR_NONE)
    {
        *this = value;
    }

    /// Destruct.
    ~Variant()
    {
        SetType(VAR_NONE);
    }

    /// Reset to empty.
    void Clear()
    {
        SetType(VAR_NONE);
    }

    /// Assign from another variant.
    Variant& operator =(const Variant& rhs);

    /// Assign from an integer.
    Variant& operator =(int rhs)
    {
        SetType(VAR_INT);
        value_.int_ = rhs;
        return *this;
    }

    /// Assign from an unsigned integer.
    Variant& operator =(unsigned rhs)
    {
        SetType(VAR_INT);
        value_.int_ = (int)rhs;
        return *this;
    }

    /// Assign from a bool.
    Variant& operator =(bool rhs)
    {
        SetType(VAR_BOOL);
        value_.bool_ = rhs;
        return *this;
    }

    /// Assign from a float.
    Variant& operator =(float rhs)
    {
        SetType(VAR_FLOAT);
        value_.float_ = rhs;
        return *this;
    }

    /// Assign from a double.
    Variant& operator = (double rhs)
    {
        SetType(VAR_DOUBLE);
        *(reinterpret_cast<double*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a string.
    Variant& operator =(const std::string& rhs)
    {
        SetType(VAR_STRING);
        *(reinterpret_cast<std::string*>(&value_)) = rhs;
        return *this;
    }

    /// Assign from a C string.
    Variant& operator =(const char* rhs)
    {
        SetType(VAR_STRING);
        *(reinterpret_cast<std::string*>(value_.ptr_)) = std::string(rhs);
        return *this;
    }

    /// Assign from a void pointer.
    Variant& operator =(void* rhs)
    {
        SetType(VAR_VOIDPTR);
        value_.ptr_ = rhs;
        return *this;
    }

    /// Assign from a RefCounted pointer. The object will be stored internally in a WeakPtr so that its expiration can be detected safely.
    Variant& operator =(RefCounted* rhs)
    {
        SetType(VAR_PTR);
        *(reinterpret_cast<WeakPtr<RefCounted>*>(value_.ptr_)) = rhs;
        return *this;
    }

    /// Test for equality with another variant.
    bool operator ==(const Variant& rhs) const;

    /// Test for equality with an integer. To return true, both the type and value must match.
    bool operator ==(int rhs) const { return type_ == VAR_INT ? value_.int_ == rhs : false; }

    /// Test for equality with an unsigned integer. To return true, both the type and value must match.
    bool operator ==(unsigned rhs) const { return type_ == VAR_INT ? value_.int_ == (int)rhs : false; }

    /// Test for equality with a bool. To return true, both the type and value must match.
    bool operator ==(bool rhs) const { return type_ == VAR_BOOL ? value_.bool_ == rhs : false; }

    /// Test for equality with a float. To return true, both the type and value must match.
    bool operator ==(float rhs) const { return type_ == VAR_FLOAT ? value_.float_ == rhs : false; }

    /// Test for equality with a double. To return true, both the type and value must match.
    bool operator ==(double rhs) const { return type_ == VAR_DOUBLE ? *(reinterpret_cast<const double*>(&value_)) == rhs : false; }

    /// Test for equality with a string. To return true, both the type and value must match.
    bool operator ==(const std::string& rhs) const
    {
        return type_ == VAR_STRING ? *(reinterpret_cast<const std::string*>(&value_)) == rhs : false;
    }

    /// Test for equality with a void pointer. To return true, both the type and value must match, with the exception that a RefCounted pointer is also allowed.
    bool operator ==(void* rhs) const
    {
        if (type_ == VAR_VOIDPTR)
            return value_.ptr_ == rhs;
        else if (type_ == VAR_PTR)
            return *(reinterpret_cast<const WeakPtr<RefCounted>*>(&value_)) == rhs;
        else
            return false;
    }

    /// Test for equality with a RefCounted pointer. To return true, both the type and value must match, with the exception that void pointer is also allowed.
    bool operator ==(RefCounted* rhs) const
    {
        if (type_ == VAR_PTR)
            return *(reinterpret_cast<const WeakPtr<RefCounted>*>(&value_)) == rhs;
        else if (type_ == VAR_VOIDPTR)
            return value_.ptr_ == rhs;
        else
            return false;
    }
    /// Test for inequality with another variant.
    bool operator !=(const Variant& rhs) const { return !(*this == rhs); }

    /// Test for inequality with an integer.
    bool operator !=(int rhs) const { return !(*this == rhs); }

    /// Test for inequality with an unsigned integer.
    bool operator !=(unsigned rhs) const { return !(*this == rhs); }

    /// Test for inequality with a bool.
    bool operator !=(bool rhs) const { return !(*this == rhs); }

    /// Test for inequality with a float.
    bool operator !=(float rhs) const { return !(*this == rhs); }

    /// Test for inequality with a double.
    bool operator !=(double rhs) const { return !(*this == rhs); }

    /// Test for inequality with a string.
    bool operator !=(const std::string& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a %VectorBuffer.
    //bool operator !=(const VectorBuffer& rhs) const { return !(*this == rhs); }

    /// Test for inequality with a pointer.
    bool operator !=(void* rhs) const { return !(*this == rhs); }

    /// Test for inequality with a RefCounted pointer.
    bool operator !=(RefCounted* rhs) const { return !(*this == rhs); }

    /// Set from typename and value strings. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(const std::string& type, const std::string& value);
    /// Set from typename and value strings. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(const char* type, const char* value);
    /// Set from type and value string. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(VariantType type, const std::string& value);
    /// Set from type and value string. Pointers will be set to null, and VariantBuffer or VariantMap types are not supported.
    void FromString(VariantType type, const char* value);

    /// Return int or zero on type mismatch. Floats and doubles are converted.
    int GetInt() const
    {
        if (type_ == VAR_INT)
            return value_.int_;
        else if (type_ == VAR_FLOAT)
            return (int)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (int)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return unsigned int or zero on type mismatch. Floats and doubles are converted.
    unsigned GetUInt() const
    {
        if (type_ == VAR_INT)
            return value_.int_;
        else if (type_ == VAR_FLOAT)
            return (unsigned)value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (unsigned)*reinterpret_cast<const double*>(&value_);
        else
            return 0;
    }

    /// Return StringHash or zero on type mismatch.
    //StringHash GetStringHash() const { return StringHash(GetUInt()); }

    /// Return bool or false on type mismatch.
    bool GetBool() const { return type_ == VAR_BOOL ? value_.bool_ : false; }

    /// Return float or zero on type mismatch. Ints and doubles are converted.
    float GetFloat() const
    {
        if (type_ == VAR_FLOAT)
            return value_.float_;
        else if (type_ == VAR_DOUBLE)
            return (float)*reinterpret_cast<const double*>(&value_);
        else if (type_ == VAR_INT)
            return (float)value_.int_;
        else
            return 0.0f;
    }

    /// Return double or zero on type mismatch. Ints and floats are converted.
    double GetDouble() const
    {
        if (type_ == VAR_DOUBLE)
            return *reinterpret_cast<const double*>(&value_);
        else if (type_ == VAR_FLOAT)
            return (double)value_.float_;
        else if (type_ == VAR_INT)
            return (double)value_.int_;
        else
            return 0.0;
    }

    /// Return string or empty on type mismatch.
	const std::string& GetString() const;

    /// Return %VectorBuffer containing the buffer or empty on type mismatch.
    //VectorBuffer GetVectorBuffer() const;

    /// Return void pointer or null on type mismatch. RefCounted pointer will be converted.
    void* GetVoidPtr() const
    {
        if (type_ == VAR_VOIDPTR)
            return value_.ptr_;
        else if (type_ == VAR_PTR)
            return *reinterpret_cast<const WeakPtr<RefCounted>*>(&value_);
        else
            return 0;
    }

    /// Return a RefCounted pointer or null on type mismatch. Will return null if holding a void pointer, as it can not be safely verified that the object is a RefCounted.
    RefCounted* GetPtr() const
    {
        return type_ == VAR_PTR ? *static_cast<const WeakPtr<RefCounted>*>(value_.ptr_) : (RefCounted*)0;
    }

    /// Return value's type.
    VariantType GetType() const { return type_; }

    /// Return value's type name.
	std::string GetTypeName() const;
    /// Convert value to string. Pointers are returned as null, and VariantBuffer or VariantMap are not supported and return empty.
	std::string ToString() const;
    /// Return true when the variant value is considered zero according to its actual type.
    bool IsZero() const;

    /// Return true when the variant is empty (i.e. not initialized yet).
    bool IsEmpty() const { return type_ == VAR_NONE; }

    /// Return the value, template version.
    template <class T> T Get() const;

    /// Return name for variant type.
    static std::string GetTypeName(VariantType type);
    /// Return variant type from type name.
    static VariantType GetTypeFromName(const std::string& typeName);
    /// Return variant type from type name.
    static VariantType GetTypeFromName(const char* typeName);

    /// Empty variant.
    static const Variant EMPTY;

private:
    /// Set new type and allocate/deallocate memory as necessary.
    void SetType(VariantType newType);

    /// Variant type.
    VariantType type_;
    /// Variant value.
    VariantValue value_;
};

/// Return variant type from type.
template <typename T> VariantType GetVariantType();

// Return variant type from concrete types
template <> inline VariantType GetVariantType<int>() { return VAR_INT; }

template <> inline VariantType GetVariantType<unsigned>() { return VAR_INT; }

template <> inline VariantType GetVariantType<bool>() { return VAR_BOOL; }

template <> inline VariantType GetVariantType<float>() { return VAR_FLOAT; }

template <> inline VariantType GetVariantType<double>() { return VAR_DOUBLE; }

template <> inline VariantType GetVariantType<std::string>() { return VAR_STRING; }


template <> CC_DLL int Variant::Get<int>() const;

template <> CC_DLL unsigned Variant::Get<unsigned>() const;

template <> CC_DLL bool Variant::Get<bool>() const;

template <> CC_DLL float Variant::Get<float>() const;

template <> CC_DLL double Variant::Get<double>() const;

template <> CC_DLL const std::string& Variant::Get<const std::string&>() const;

template <> CC_DLL void* Variant::Get<void*>() const;

template <> CC_DLL RefCounted* Variant::Get<RefCounted*>() const;
template <> CC_DLL std::string Variant::Get<std::string>() const;

}
