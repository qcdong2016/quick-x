/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2007      Scott Lembcke
Copyright (c) 2010      Lam Pham

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "CCPointExtension.h"
#include "ccMacros.h" // FLT_EPSILON
#include <stdio.h>

NS_CC_BEGIN

#define kCCPointEpsilon FLT_EPSILON

float
ccpLength(const Vec2& v)
{
    return v.getLength();
}

float
ccpDistance(const Vec2& v1, const Vec2& v2)
{
    return (v1 - v2).getLength();
}

Vec2
ccpNormalize(const Vec2& v)
{
    return v.normalize();
}

Vec2
ccpForAngle(const float a)
{
    return Vec2::forAngle(a);
}

float
ccpToAngle(const Vec2& v)
{
    return v.getAngle();
}

Vec2 ccpLerp(const Vec2& a, const Vec2& b, float alpha)
{
    return a.lerp(b, alpha);
}

float clampf(float value, float min_inclusive, float max_inclusive)
{
    if (min_inclusive > max_inclusive) {
        CC_SWAP(min_inclusive, max_inclusive, float);
    }
    return value < min_inclusive ? min_inclusive : value < max_inclusive? value : max_inclusive;
}

Vec2 ccpClamp(const Vec2& p, const Vec2& min_inclusive, const Vec2& max_inclusive)
{
    return ccp(clampf(p.x,min_inclusive.x,max_inclusive.x), clampf(p.y, min_inclusive.y, max_inclusive.y));
}

Vec2 ccpFromSize(const CCSize& s)
{
    return Vec2(s);
}

Vec2 ccpCompOp(const Vec2& p, float (*opFunc)(float))
{
    return ccp(opFunc(p.x), opFunc(p.y));
}

bool ccpFuzzyEqual(const Vec2& a, const Vec2& b, float var)
{
	return a.fuzzyEquals(b, var);
}

Vec2 ccpCompMult(const Vec2& a, const Vec2& b)
{
    return ccp(a.x * b.x, a.y * b.y);
}

float ccpAngleSigned(const Vec2& a, const Vec2& b)
{
	return a.getAngle(b);
}

Vec2 ccpRotateByAngle(const Vec2& v, const Vec2& pivot, float angle)
{
	return v.rotateByAngle(pivot, angle);
}


bool ccpSegmentIntersect(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T )
        && (S >= 0.0f && S <= 1.0f && T >= 0.0f && T <= 1.0f) )
        return true;

    return false;
}

Vec2 ccpIntersectPoint(const Vec2& A, const Vec2& B, const Vec2& C, const Vec2& D)
{
    float S, T;

    if( ccpLineIntersect(A, B, C, D, &S, &T) )
    {
        // Point of intersection
        Vec2 P;
        P.x = A.x + S * (B.x - A.x);
        P.y = A.y + S * (B.y - A.y);
        return P;
    }

    return CCPointZero;
}

bool ccpLineIntersect(const Vec2& A, const Vec2& B, 
                      const Vec2& C, const Vec2& D,
                      float *S, float *T)
{
    // FAIL: Line undefined
    if ( (A.x==B.x && A.y==B.y) || (C.x==D.x && C.y==D.y) )
    {
        return false;
    }
    const float BAx = B.x - A.x;
    const float BAy = B.y - A.y;
    const float DCx = D.x - C.x;
    const float DCy = D.y - C.y;
    const float ACx = A.x - C.x;
    const float ACy = A.y - C.y;

    const float denom = DCy*BAx - DCx*BAy;

    *S = DCx*ACy - DCy*ACx;
    *T = BAx*ACy - BAy*ACx;

    if (denom == 0)
    {
        if (*S == 0 || *T == 0)
        { 
            // Lines incident
            return true;   
        }
        // Lines parallel and not incident
        return false;
    }

    *S = *S / denom;
    *T = *T / denom;

    // Point of intersection
    // CGPoint P;
    // P.x = A.x + *S * (B.x - A.x);
    // P.y = A.y + *S * (B.y - A.y);

    return true;
}

float ccpAngle(const Vec2& a, const Vec2& b)
{
    float angle = acosf(ccpDot(ccpNormalize(a), ccpNormalize(b)));
    if( fabs(angle) < kCCPointEpsilon ) return 0.f;
    return angle;
}

NS_CC_END
