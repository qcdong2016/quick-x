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

struct CC_DLL cc_timeval
{
#ifdef __native_client__
	time_t    tv_sec;        // seconds
#else
	long    tv_sec;        // seconds
#endif
	int tv_usec;    // microSeconds
};

class CC_DLL CCTime
{
public:
	static void sleep(unsigned mSec);
	static int gettimeofdayCocos2d(struct cc_timeval *tp, void *tzp);
	static double timersubCocos2d(struct cc_timeval *start, struct cc_timeval *end);
};

}
unsigned int class_hash_code(const std::type_info& info);
#define CLASS_HASH_CODE class_hash_code

#endif//_CCTIMER_H_