#include "CCDevice.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


NS_CC_BEGIN
static const int kMaxLogLen = 16 * 1024;
static char logbuff[sizeof(char) * (kMaxLogLen + 1)];

void CCDevice::log(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	logV(fmt, args);
	va_end(args);
}

void CCDevice::logV(const char * fmt, va_list args)
{
	vsnprintf(logbuff, sizeof(logbuff), fmt, args);
	logS(logbuff);
}

bool cocos2d::CCDevice::isIOS64bit()
{
	// copy from old quick lua_cocos2dx_manual.cpp
	TargetPlatform platform = cocos2d::CCDevice::getTargetPlatform();
	if (kTargetIphone == platform || kTargetIpad == platform)
	{
#if defined(__LP64__)
		return true;
#endif
	}

	return false;
}

void CCLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	CCDevice::logV(fmt, args);
	va_end(args);
}

NS_CC_END


