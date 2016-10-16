#include "platform/CCDevice.h"
#include "android/jni/DPIJni.h"
#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include <jni.h>
#include <android/log.h>
#include <stdio.h>

NS_CC_BEGIN

int CCDevice::getDPI()
{
    static int dpi = -1;
    if (dpi == -1)
    {
        dpi = (int)getDPIJNI();
    }
    return dpi;
}

void CCDevice::logS(const char* s)
{
    __android_log_print(ANDROID_LOG_DEBUG, "cocos2d-x", "%s", s);
}

void CCDevice::showMessageBox(const char * pszMsg, const char * pszTitle)
{
    showDialogJNI(pszMsg, pszTitle);
}

NS_CC_END
