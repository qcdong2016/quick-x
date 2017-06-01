#include "CCTimer.h"
#include "SDL_timer.h"
#include "CCPlatformDefine.h"
#include "CCPlatformMacros.h"
#include <ctype.h>
#if BUILD_FOR(WIN32)
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
    
void Time::sleep(unsigned mSec)
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
