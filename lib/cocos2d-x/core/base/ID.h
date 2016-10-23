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

// from Urho3D StringHash

#pragma once

#include <string>
#include "CCPlatformDefine.h"


namespace cocos2d
{

/// 32-bit hash value for a string.
class CC_DLL ID
{
public:
    /// Construct with zero value.
    ID() :
        value_(0)
    {
    }

    /// Copy-construct from another hash.
    ID(const ID& rhs) :
        value_(rhs.value_)
    {
    }

    /// Construct with an initial value.
    explicit ID(unsigned value) :
        value_(value)
    {
    }

    /// Construct from a C string case-insensitively.
    ID(const char* str);
    /// Construct from a string case-insensitively.
    ID(const std::string& str);

    /// Assign from another hash.
    ID& operator =(const ID& rhs)
    {
        value_ = rhs.value_;
        return *this;
    }

    /// Add a hash.
    ID operator +(const ID& rhs) const
    {
        ID ret;
        ret.value_ = value_ + rhs.value_;
        return ret;
    }

    /// Add-assign a hash.
    ID& operator +=(const ID& rhs)
    {
        value_ += rhs.value_;
        return *this;
    }

    /// Test for equality with another hash.
    bool operator ==(const ID& rhs) const { return value_ == rhs.value_; }

    /// Test for inequality with another hash.
    bool operator !=(const ID& rhs) const { return value_ != rhs.value_; }

    /// Test if less than another hash.
    bool operator <(const ID& rhs) const { return value_ < rhs.value_; }

    /// Test if greater than another hash.
    bool operator >(const ID& rhs) const { return value_ > rhs.value_; }

    /// Return true if nonzero hash value.
    operator bool() const { return value_ != 0; }

    /// Return hash value.
    unsigned value() const { return value_; }

    /// Return as string.
	std::string toString() const;

    /// Return hash value for HashSet & HashMap.
    unsigned toHash() const { return value_; }

    /// Calculate hash value case-insensitively from a C string.
    static unsigned calculate(const char* str);

    /// Zero hash.
    static const ID ZERO;

private:
    /// Hash value.
    unsigned value_;
};

}
