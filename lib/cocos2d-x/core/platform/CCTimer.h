#ifndef _CCTIMER_H_
#define _CCTIMER_H_
#include "CCPlatformDefine.h"
#include <typeinfo>

namespace cocos2d
{

//high resolution
class TimerHiRes
{
public:
    TimerHiRes();

    void reset();
    long long elapsed();

protected:
    long long _startTime;
};

class CC_DLL Time
{
public:
	static void sleep(unsigned mSec);
};

}
unsigned int class_hash_code(const std::type_info& info);
#define CLASS_HASH_CODE class_hash_code

#endif//_CCTIMER_H_
