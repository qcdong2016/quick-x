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

#include <cstring>
#include "Variant.h"

namespace cocos2d
{

const Variant Variant::EMPTY;

static const char* typeNames[] =
{
    "None",
    "Int",
    "Bool",
    "Float",
	"Double",
	"String",
	"Ptr",
	"VoidPtr",

    0
};

Variant& Variant::operator =(const Variant& rhs)
{
    SetType(rhs.GetType());

    switch (type_)
    {
    case VAR_STRING:
        *(static_cast<std::string*>(value_.ptr_)) = *(static_cast<const std::string*>(rhs.value_.ptr_));
        break;

    case VAR_PTR:
        *(static_cast<WeakPtr<RefCounted>*>(value_.ptr_)) = *(static_cast<const WeakPtr<RefCounted>*>(rhs.value_.ptr_));
        break;

    default:
        value_ = rhs.value_;
        break;
    }

    return *this;
}

bool Variant::operator ==(const Variant& rhs) const
{
    if (type_ == VAR_VOIDPTR || type_ == VAR_PTR)
        return GetVoidPtr() == rhs.GetVoidPtr();
    else if (type_ != rhs.type_)
        return false;

    switch (type_)
    {
    case VAR_INT:
        return value_.int_ == rhs.value_.int_;

    case VAR_BOOL:
        return value_.bool_ == rhs.value_.bool_;

    case VAR_FLOAT:
        return value_.float_ == rhs.value_.float_;

	case VAR_STRING:
        return *(static_cast<const std::string*>(value_.ptr_)) == *(static_cast<const std::string*>(rhs.value_.ptr_));

    case VAR_DOUBLE:
        return *(reinterpret_cast<const double*>(&value_)) == *(reinterpret_cast<const double*>(&rhs.value_));

    default:
        return true;
    }
}

void Variant::FromString(const std::string& type, const std::string& value)
{
    return FromString(GetTypeFromName(type), value.c_str());
}

void Variant::FromString(const char* type, const char* value)
{
    return FromString(GetTypeFromName(type), value);
}

void Variant::FromString(VariantType type, const std::string& value)
{
    return FromString(type, value.c_str());
}
bool ToBool(const char* source)
{
	unsigned length = strlen(source);

	for (unsigned i = 0; i < length; ++i)
	{
		char c = (char)tolower(source[i]);
		if (c == 't' || c == 'y' || c == '1')
			return true;
		else if (c != ' ' && c != '\t')
			break;
	}

	return false;
}
float ToFloat(const char* source)
{
	if (!source)
		return 0;

	return (float)strtod(source, 0);
}
int ToInt(const char* source, int base)
{
	if (!source)
		return 0;

	// Shield against runtime library assert by converting illegal base values to 0 (autodetect)
	if (base < 2 || base > 36)
		base = 0;

	return (int)strtol(source, 0, base);
}
double ToDouble(const char* source)
{
	if (!source)
		return 0;

	return strtod(source, 0);
}
void Variant::FromString(VariantType type, const char* value)
{
    switch (type)
    {
    case VAR_INT:
        *this = ToInt(value, 10);
        break;

    case VAR_BOOL:
        *this = ToBool(value);
        break;

    case VAR_FLOAT:
        *this = ToFloat(value);
        break;

    case VAR_STRING:
        *this = value;
        break;

    case VAR_VOIDPTR:
        // From string to void pointer not supported, set to null
        *this = (void*)0;
        break;

        break;

    case VAR_PTR:
        // From string to RefCounted pointer not supported, set to null
        *this = (RefCounted*)0;
        break;

    case VAR_DOUBLE:
        *this = ToDouble(value);
        break;

    default:
        SetType(VAR_NONE);
    }
}

static std::string STRING_EMPTY;

const std::string& Variant::GetString() const
{
	return type_ == VAR_STRING ? *reinterpret_cast<const std::string*>(value_.ptr_) : STRING_EMPTY;
}

std::string Variant::GetTypeName() const
{
    return typeNames[type_];
}

std::string Variant::ToString() const
{
    switch (type_)
    {
    case VAR_INT:
        return std::to_string(value_.int_);

    case VAR_BOOL:
        return std::to_string(value_.bool_);

    case VAR_FLOAT:
        return std::to_string(value_.float_);

    case VAR_STRING:
		return *static_cast<const std::string*>(value_.ptr_);

    case VAR_VOIDPTR:
    case VAR_PTR:
        // Pointer serialization not supported (convert to null)
        return std::string();

    case VAR_DOUBLE:
        return std::to_string(*reinterpret_cast<const double*>(&value_));

    default:
        // VAR_RESOURCEREF, VAR_RESOURCEREFLIST, VAR_VARIANTVECTOR, VAR_STRINGVECTOR, VAR_VARIANTMAP
        // Reference string serialization requires typehash-to-name mapping from the context. Can not support here
        // Also variant map or vector string serialization is not supported. XML or binary save should be used instead
        return std::string();
    }
}

bool Variant::IsZero() const
{
    switch (type_)
    {
    case VAR_INT:
        return value_.int_ == 0;

    case VAR_BOOL:
        return value_.bool_ == false;

    case VAR_FLOAT:
        return value_.float_ == 0.0f;

    case VAR_STRING:
		return static_cast<const std::string*>(value_.ptr_)->empty();

    case VAR_VOIDPTR:
        return value_.ptr_ == 0;

    case VAR_PTR:
        return *static_cast<const WeakPtr<RefCounted>*>(value_.ptr_) == (RefCounted*)0;

    case VAR_DOUBLE:
        return *reinterpret_cast<const double*>(&value_) == 0.0;

    default:
        return true;
    }
}

void Variant::SetType(VariantType newType)
{
    if (type_ == newType)
        return;

    switch (type_)
    {
    case VAR_STRING:
        delete (static_cast<std::string*>(value_.ptr_));
        break;

    case VAR_PTR:
        delete (static_cast<WeakPtr<RefCounted>*>(value_.ptr_));
        break;

    default:
        break;
    }

    type_ = newType;

    switch (type_)
    {
    case VAR_STRING:
		value_.ptr_ = new std::string();
        break;

    case VAR_PTR:
		value_.ptr_ = new WeakPtr<RefCounted>();
        break;

    default:
        break;
    }
}

template <> int Variant::Get<int>() const
{
    return GetInt();
}

template <> unsigned Variant::Get<unsigned>() const
{
    return GetUInt();
}

template <> bool Variant::Get<bool>() const
{
    return GetBool();
}

template <> float Variant::Get<float>() const
{
    return GetFloat();
}

template <> double Variant::Get<double>() const
{
    return GetDouble();
}

template <> const std::string& Variant::Get<const std::string&>() const
{
    return GetString();
}


template <> void* Variant::Get<void*>() const
{
    return GetVoidPtr();
}

template <> RefCounted* Variant::Get<RefCounted*>() const
{
    return GetPtr();
}

template <> std::string Variant::Get<std::string>() const
{
    return GetString();
}

std::string Variant::GetTypeName(VariantType type)
{
    return typeNames[type];
}

VariantType Variant::GetTypeFromName(const std::string& typeName)
{
    return GetTypeFromName(typeName.c_str());
}

unsigned GetStringListIndex(const char* value, const char** strings, unsigned defaultIndex)
{
	unsigned i = 0;

	while (strings[i])
	{
		if (!strcmp(value, strings[i]))
			return i;
		++i;
	}

	return defaultIndex;
}

VariantType Variant::GetTypeFromName(const char* typeName)
{
    return (VariantType)GetStringListIndex(typeName, typeNames, VAR_NONE);
}

}
