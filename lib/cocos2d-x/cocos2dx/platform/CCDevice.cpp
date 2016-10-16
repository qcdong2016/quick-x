#include "CCDevice.h"

#include <stdarg.h>
#include <stdio.h>
#include <string.h>


NS_CC_BEGIN
static const int kMaxLogLen = 16 * 1024;
static char logbuff[sizeof(char) * (kMaxLogLen + 1)];

void CCDevice::Log(const char * fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	LogV(fmt, args);
	va_end(args);
}

void CCDevice::LogV(const char * fmt, va_list args)
{
	vsnprintf(logbuff, sizeof(logbuff), fmt, args);
	LogS(logbuff);
}

void CCLog(const char* fmt, ...)
{
	va_list args;
	va_start(args, fmt);
	CCDevice::LogV(fmt, args);
	va_end(args);
}

NS_CC_END


