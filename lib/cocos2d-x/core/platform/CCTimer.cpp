#include "CCTimer.h"
#include "SDL_timer.h"
#include "CCPlatformDefine.h"
#include "CCPlatformMacros.h"
#include <ctype.h>
#if OS_IS(WIN32)
#include <winsock.h>
#endif

namespace cocos2d
{

static long long highResolutionTick()
{
    return SDL_GetPerformanceCounter();
}

static long long highResolutionFrequency()
{
    return SDL_GetPerformanceFrequency();
}

TimerHiRes::TimerHiRes()
{
    reset();
}

void TimerHiRes::reset()
{
    _startTime = highResolutionTick();
}

long long TimerHiRes::elapsed()
{
    long long currentTime = highResolutionTick();
    long long elapsedTime = currentTime - _startTime;

    if (elapsedTime < 0)
        elapsedTime = 0;

    return (elapsedTime * 1000000LL) / highResolutionFrequency();
}


int CCTime::gettimeofdayCocos2d(struct cc_timeval *tp, void *tzp)
{
	CC_UNUSED_PARAM(tzp);
#if OS_IS(WIN32)
	if (tp)
	{
		LARGE_INTEGER liTime, liFreq;
		QueryPerformanceFrequency(&liFreq);
		QueryPerformanceCounter(&liTime);
		tp->tv_sec = (long)(liTime.QuadPart / liFreq.QuadPart);
		tp->tv_usec = (long)(liTime.QuadPart * 1000000.0 / liFreq.QuadPart - tp->tv_sec * 1000000.0);
	}
#endif
	return 0;
}

double CCTime::timersubCocos2d(struct cc_timeval *start, struct cc_timeval *end)
{
	if (!start || !end)
	{
		return 0;
	}

	return ((end->tv_sec*1000.0 + end->tv_usec / 1000.0) - (start->tv_sec*1000.0 + start->tv_usec / 1000.0));
}


void CCTime::sleep(unsigned mSec)
{
	SDL_Delay(mSec);
} 
}

// from 2dx
static unsigned int _Hash(const char *key)
{
	unsigned int len = strlen(key);
	const char *end = key + len;
	unsigned int hash;

	for (hash = 0; key < end; key++)
	{
		hash *= 16777619;
		hash ^= (unsigned int)(unsigned char)toupper(*key);
	}
	return (hash);
}

unsigned int class_hash_code(const std::type_info& info)
{
	// hash name() to size_t value by pseudorandomizing transform
	return _Hash(info.name());
}
