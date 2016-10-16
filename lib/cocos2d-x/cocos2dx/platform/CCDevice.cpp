
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "CCDevice.h"


#if (CC_TARGET_PLATFORM != CC_PLATFORM_ANDROID)


/// The max length of CCLog message.
static const int kMaxLogLen = 16*1024;
static char logbuff[sizeof(char) * (kMaxLogLen + 1)];

NS_CC_BEGIN



void CCDevice::Log(const char * pszFormat, ...)
{
    printf("Cocos2d: ");
    va_list ap;
    va_start(ap, pszFormat);
    memset(logbuff, 0, sizeof(logbuff));
    vsnprintf(logbuff, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", logbuff);
    printf("\n");
    fflush(stdout);
}


void CCDevice::LuaLog(const char * pszLog)
{
    printf("Cocos2d: ");
    if (strlen(pszLog) > 65536)
    {
        printf("Cocos2d: [WARNING] log too long, max size is %0.2fKB, actual size is %0.2fKB\n", (float)kMaxLogLen / 1024.0f, (float)strlen(pszLog) / 1024.0f);
        memset(logbuff, 0, sizeof(logbuff));
        strncpy(logbuff, pszLog, kMaxLogLen);
        puts(logbuff);
    }
    else
    {
        puts(pszLog);
    }
}

NS_CC_END

#endif

void CCLog(const char* pszFormat, ...)
{
    printf("Cocos2d: ");
    va_list ap;
    va_start(ap, pszFormat);
    memset(logbuff, 0, sizeof(logbuff));
    vsnprintf(logbuff, kMaxLogLen, pszFormat, ap);
    va_end(ap);
    printf("%s", logbuff);
    printf("\n");
    fflush(stdout);
}
