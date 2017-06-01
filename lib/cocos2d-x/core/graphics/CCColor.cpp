
#include "CCColor.h"
#include "base/MathDefs.h"

NS_CC_BEGIN

unsigned Color::toUInt() const
{
	unsigned ur = (unsigned)Clamp(((int)(r * 255.0f)), 0, 255);
	unsigned ug = (unsigned)Clamp(((int)(g * 255.0f)), 0, 255);
	unsigned ub = (unsigned)Clamp(((int)(b * 255.0f)), 0, 255);
	unsigned ua = (unsigned)Clamp(((int)(a * 255.0f)), 0, 255);
	return (ua << 24) | (ub << 16) | (ug << 8) | ur;
}

const Color Color::WHITE;
const Color Color::GRAY(0.5f, 0.5f, 0.5f);
const Color Color::BLACK(0.0f, 0.0f, 0.0f);
const Color Color::RED(1.0f, 0.0f, 0.0f);
const Color Color::GREEN(0.0f, 1.0f, 0.0f);
const Color Color::BLUE(0.0f, 0.0f, 1.0f);
const Color Color::CYAN(0.0f, 1.0f, 1.0f);
const Color Color::MAGENTA(1.0f, 0.0f, 1.0f);
const Color Color::YELLOW(1.0f, 1.0f, 0.0f);
const Color Color::TRANSPARENT(0.0f, 0.0f, 0.0f, 0.0f);

NS_CC_END
