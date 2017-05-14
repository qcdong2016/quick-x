#pragma once

#include "CCPlatformDefine.h"
#include "CCPlatformMacros.h"
#include "base/MathDefs.h"

NS_CC_BEGIN

class CCSize;

class CC_DLL Vec2
{
public:
	float x;
	float y;

public:
	Vec2();
	Vec2(float x, float y);
	/**
	* @lua NA
	*/
	Vec2(const Vec2& other);
	/**
	* @lua NA
	*/
	Vec2(const CCSize& size);
	/**
	* @lua NA
	*/
	Vec2& operator= (const Vec2& other);
	/**
	* @lua NA
	*/
	Vec2& operator= (const CCSize& size);
	/**
	* @lua NA
	*/
	Vec2 operator+(const Vec2& right) const;
	/**
	* @lua NA
	*/
	Vec2 operator-(const Vec2& right) const;
	/**
	* @lua NA
	*/
	Vec2 operator-() const;
	/**
	* @lua NA
	*/
	Vec2 operator*(float a) const;
	/**
	* @lua NA
	*/
	Vec2 operator/(float a) const;
	/**
	* @lua NA
	*/
	void setPoint(float x, float y);
	bool equals(const Vec2& target) const;

	/** @returns if points have fuzzy equality which means equal with some degree of variance.
	* @since v2.1.4
	* @lua NA
	*/
	bool fuzzyEquals(const Vec2& target, float variance) const;

	/** Calculates distance between point an origin
	* @return float
	* @since v2.1.4
	* @lua NA
	*/
	inline float getLength() const {
		return sqrtf(x*x + y*y);
	};

	/** Calculates the square length of a CCPoint (not calling sqrt() )
	* @return float
	* @since v2.1.4
	* @lua NA
	*/
	inline float getLengthSq() const {
		return dot(*this); //x*x + y*y;
	};

	/** Calculates the square distance between two points (not calling sqrt() )
	@return float
	@since v2.1.4
	*/
	inline float getDistanceSq(const Vec2& other) const {
		return (*this - other).getLengthSq();
	};

	/** Calculates the distance between two points
	@return float
	@since v2.1.4
	*/
	inline float getDistance(const Vec2& other) const {
		return (*this - other).getLength();
	};

	/** @returns the angle in radians between this vector and the x axis
	@since v2.1.4
	*/
	inline float getAngle() const {
		return Atan2(y, x);
	};

	/** @returns the angle in radians between two vector directions
	@since v2.1.4
	*/
	float getAngle(const Vec2& other) const;

	/** Calculates dot product of two points.
	@return float
	@since v2.1.4
	*/
	inline float dot(const Vec2& other) const {
		return x*other.x + y*other.y;
	};

	/** Calculates cross product of two points.
	@return float
	@since v2.1.4
	*/
	inline float cross(const Vec2& other) const {
		return x*other.y - y*other.x;
	};

	/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
	@return CCPoint
	@since v2.1.4
	*/
	inline Vec2 getPerp() const {
		return Vec2(-y, x);
	};

	/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
	@return CCPoint
	@since v2.1.4
	*/
	inline Vec2 getRPerp() const {
		return Vec2(y, -x);
	};

	/** Calculates the projection of this over other.
	@return CCPoint
	@since v2.1.4
	*/
	inline Vec2 project(const Vec2& other) const {
		return other * (dot(other) / other.dot(other));
	};

	/** Complex multiplication of two points ("rotates" two points).
	@return CCPoint vector with an angle of this.getAngle() + other.getAngle(),
	and a length of this.getLength() * other.getLength().
	@since v2.1.4
	*/
	inline Vec2 rotate(const Vec2& other) const {
		return Vec2(x*other.x - y*other.y, x*other.y + y*other.x);
	};

	/** Unrotates two points.
	@return CCPoint vector with an angle of this.getAngle() - other.getAngle(),
	and a length of this.getLength() * other.getLength().
	@since v2.1.4
	*/
	inline Vec2 unrotate(const Vec2& other) const {
		return Vec2(x*other.x + y*other.y, y*other.x - x*other.y);
	};

	/** Returns point multiplied to a length of 1.
	* If the point is 0, it returns (1, 0)
	@return CCPoint
	@since v2.1.4
	*/
	inline Vec2 normalize() const {
		float length = getLength();
		if (length == 0.) return Vec2(1.f, 0);
		return *this / getLength();
	};

	/** Linear Interpolation between two points a and b
	@returns
	alpha == 0 ? a
	alpha == 1 ? b
	otherwise a value between a..b
	@since v2.1.4
	*/
	inline Vec2 lerp(const Vec2& other, float alpha) const {
		return *this * (1.f - alpha) + other * alpha;
	};

	/** Rotates a point counter clockwise by the angle around a pivot
	@param pivot is the pivot, naturally
	@param angle is the angle of rotation ccw in radians
	@returns the rotated point
	@since v2.1.4
	*/
	Vec2 rotateByAngle(const Vec2& pivot, float angle) const;

	static inline Vec2 forAngle(const float a)
	{
		return Vec2(Cos(a), Sin(a));
	}
};


/** Helper macro that creates a CCPoint
@return CCPoint
@since v0.7.2
*/
#define ccp(__X__,__Y__) cocos2d::CCPointMake((float)(__X__), (float)(__Y__))

/** Returns opposite of point.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpNeg(const Vec2& v)
{
	return -v;
}

/** Calculates sum of two points.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpAdd(const Vec2& v1, const Vec2& v2)
{
	return v1 + v2;
}

/** Calculates difference of two points.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpSub(const Vec2& v1, const Vec2& v2)
{
	return v1 - v2;
}

/** Returns point multiplied by given factor.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpMult(const Vec2& v, const float s)
{
	return v * s;
}

/** Calculates midpoint between two points.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpMidpoint(const Vec2& v1, const Vec2& v2)
{
	return (v1 + v2) / 2.f;
}

/** Calculates dot product of two points.
@return float
@since v0.7.2
*/
static inline float
ccpDot(const Vec2& v1, const Vec2& v2)
{
	return v1.dot(v2);
}

/** Calculates cross product of two points.
@return float
@since v0.7.2
*/
static inline float
ccpCross(const Vec2& v1, const Vec2& v2)
{
	return v1.cross(v2);
}

/** Calculates perpendicular of v, rotated 90 degrees counter-clockwise -- cross(v, perp(v)) >= 0
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpPerp(const Vec2& v)
{
	return v.getPerp();
}

/** Calculates perpendicular of v, rotated 90 degrees clockwise -- cross(v, rperp(v)) <= 0
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpRPerp(const Vec2& v)
{
	return v.getRPerp();
}

/** Calculates the projection of v1 over v2.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpProject(const Vec2& v1, const Vec2& v2)
{
	return v1.project(v2);
}

/** Rotates two points.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpRotate(const Vec2& v1, const Vec2& v2)
{
	return v1.rotate(v2);
}

/** Unrotates two points.
@return CCPoint
@since v0.7.2
*/
static inline Vec2
ccpUnrotate(const Vec2& v1, const Vec2& v2)
{
	return v1.unrotate(v2);
}

/** Calculates the square length of a CCPoint (not calling sqrt() )
@return float
@since v0.7.2
*/
static inline float
ccpLengthSQ(const Vec2& v)
{
	return v.getLengthSq();
}


/** Calculates the square distance between two points (not calling sqrt() )
@return float
@since v1.1
*/
static inline float
ccpDistanceSQ(const Vec2 p1, const Vec2 p2)
{
	return (p1 - p2).getLengthSq();
}


/** Calculates distance between point an origin
@return float
@since v0.7.2
*/
float CC_DLL ccpLength(const Vec2& v);

/** Calculates the distance between two points
@return float
@since v0.7.2
*/
float CC_DLL ccpDistance(const Vec2& v1, const Vec2& v2);

/** Returns point multiplied to a length of 1.
@return CCPoint
@since v0.7.2
*/
Vec2 CC_DLL ccpNormalize(const Vec2& v);

/** Converts radians to a normalized vector.
@return CCPoint
@since v0.7.2
*/
Vec2 CC_DLL ccpForAngle(const float a);

/** Converts a vector to radians.
@return float
@since v0.7.2
*/
float CC_DLL ccpToAngle(const Vec2& v);


/** Clamp a value between from and to.
@since v0.99.1
*/
float CC_DLL clampf(float value, float min_inclusive, float max_inclusive);

/** Clamp a point between from and to.
@since v0.99.1
*/
Vec2 CC_DLL ccpClamp(const Vec2& p, const Vec2& from, const Vec2& to);

/** Quickly convert CCSize to a CCPoint
@since v0.99.1
*/
Vec2 CC_DLL ccpFromSize(const CCSize& s);

/** Run a math operation function on each point component
* absf, fllorf, ceilf, roundf
* any function that has the signature: float func(float);
* For example: let's try to take the floor of x,y
* ccpCompOp(p,floorf);
@since v0.99.1
*/
Vec2 CC_DLL ccpCompOp(const Vec2& p, float(*opFunc)(float));

/** Linear Interpolation between two points a and b
@returns
alpha == 0 ? a
alpha == 1 ? b
otherwise a value between a..b
@since v0.99.1
*/
Vec2 CC_DLL ccpLerp(const Vec2& a, const Vec2& b, float alpha);


/** @returns if points have fuzzy equality which means equal with some degree of variance.
@since v0.99.1
*/
bool CC_DLL ccpFuzzyEqual(const Vec2& a, const Vec2& b, float variance);


/** Multiplies a and b components, a.x*b.x, a.y*b.y
@returns a component-wise multiplication
@since v0.99.1
*/
Vec2 CC_DLL ccpCompMult(const Vec2& a, const Vec2& b);

/** @returns the signed angle in radians between two vector directions
@since v0.99.1
*/
float CC_DLL ccpAngleSigned(const Vec2& a, const Vec2& b);

/** @returns the angle in radians between two vector directions
@since v0.99.1
*/
float CC_DLL ccpAngle(const Vec2& a, const Vec2& b);

/** Rotates a point counter clockwise by the angle around a pivot
@param v is the point to rotate
@param pivot is the pivot, naturally
@param angle is the angle of rotation cw in radians
@returns the rotated point
@since v0.99.1
*/
Vec2 CC_DLL ccpRotateByAngle(const Vec2& v, const Vec2& pivot, float angle);

/** A general line-line intersection test
@param p1
is the startpoint for the first line P1 = (p1 - p2)
@param p2
is the endpoint for the first line P1 = (p1 - p2)
@param p3
is the startpoint for the second line P2 = (p3 - p4)
@param p4
is the endpoint for the second line P2 = (p3 - p4)
@param s
is the range for a hitpoint in P1 (pa = p1 + s*(p2 - p1))
@param t
is the range for a hitpoint in P3 (pa = p2 + t*(p4 - p3))
@return bool
indicating successful intersection of a line
note that to truly test intersection for segments we have to make
sure that s & t lie within [0..1] and for rays, make sure s & t > 0
the hit point is        p3 + t * (p4 - p3);
the hit point also is    p1 + s * (p2 - p1);
@since v0.99.1
*/
bool CC_DLL ccpLineIntersect(const Vec2& p1, const Vec2& p2,
	const Vec2& p3, const Vec2& p4,
	float *s, float *t);

/*
ccpSegmentIntersect returns YES if Segment A-B intersects with segment C-D
@since v1.0.0
*/
bool CC_DLL ccpSegmentIntersect(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D);

/*
ccpIntersectPoint returns the intersection point of line A-B, C-D
@since v1.0.0
*/
Vec2 CC_DLL ccpIntersectPoint(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D);

NS_CC_END