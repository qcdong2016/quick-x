#pragma once

#include "CCPlatformDefine.h"
#include "CCPlatformMacros.h"

NS_CC_BEGIN

class CC_DLL Color
{
public:
	Color() : r(1.0f), g(1.0f), b(1.0f), a(1.0f) { }

	Color(const Color& color) : r(color.r), g(color.g), b(color.b), a(color.a) { }

	Color(const Color& color, float a) : r(color.r), g(color.g), b(color.b), a(a) { }

	Color(float r_, float g_, float b_) : r(r_), g(g_), b(b_), a(1.0f) { }

	Color(float r_, float g_, float b_, float a_) : r(r_), g(g_), b(b_), a(a_) { }

	explicit Color(const float* data) : r(data[0]), g(data[1]), b(data[2]), a(data[3]) { }

	Color& operator =(const Color& rhs) { r = rhs.r; g = rhs.g; b = rhs.b; a = rhs.a; return *this; }

	bool operator ==(const Color& rhs) const { return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a; }

	bool operator !=(const Color& rhs) const { return r != rhs.r || g != rhs.g || b != rhs.b || a != rhs.a; }

	Color operator *(float rhs) const { return Color(r * rhs, g * rhs, b * rhs, a * rhs); }

	Color operator +(const Color& rhs) const { return Color(r + rhs.r, g + rhs.g, b + rhs.b, a + rhs.a); }

	Color operator -() const { return Color(-r, -g, -b, -a); }

	Color operator -(const Color& rhs) const { return Color(r - rhs.r, g - rhs.g, b - rhs.b, a - rhs.a); }

	Color& operator +=(const Color& rhs) { r += rhs.r; g += rhs.g; b += rhs.b; a += rhs.a; return *this; }

	unsigned toUInt() const;

	float r, g, b, a;

	static const Color WHITE;
	static const Color GRAY;
	static const Color BLACK;
	static const Color RED;
	static const Color GREEN;
	static const Color BLUE;
	static const Color CYAN;
	static const Color MAGENTA;
	static const Color YELLOW;
	static const Color TRANSPARENT;
};

NS_CC_END
