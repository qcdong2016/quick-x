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


ccLanguageType CCDevice::getCurrentLanguage()
{
    std::string languageName = getCurrentLanguageJNI();
    const char* pLanguageName = languageName.c_str();
    ccLanguageType ret = kLanguageEnglish;
    
    // modify by guorui.chen
    // added current language is simple or traditional
    // if (0 == strcmp("zh", pLanguageName))
    // {
    //     ret = kLanguageChinese;
    // }
    if (0 == strcmp("zh-Hans", pLanguageName))
    {
        ret = kLanguageChinese;
    }
    else if (0 == strcmp("zh-Hant", pLanguageName))
    {
        ret = kLanguageChineseTW;
    }
    else if (0 == strcmp("en", pLanguageName))
    {
        ret = kLanguageEnglish;
    }
    else if (0 == strcmp("fr", pLanguageName))
    {
        ret = kLanguageFrench;
    }
    else if (0 == strcmp("it", pLanguageName))
    {
        ret = kLanguageItalian;
    }
    else if (0 == strcmp("de", pLanguageName))
    {
        ret = kLanguageGerman;
    }
    else if (0 == strcmp("es", pLanguageName))
    {
        ret = kLanguageSpanish;
    }
    else if (0 == strcmp("nl", pLanguageName))
    {
        ret = kLanguageDutch;
    }
    else if (0 == strcmp("ru", pLanguageName))
    {
        ret = kLanguageRussian;
    }
    else if (0 == strcmp("ko", pLanguageName))
    {
        ret = kLanguageKorean;
    }
    else if (0 == strcmp("ja", pLanguageName))
    {
        ret = kLanguageJapanese;
    }
    else if (0 == strcmp("hu", pLanguageName))
    {
        ret = kLanguageHungarian;
    }
    else if (0 == strcmp("pt", pLanguageName))
    {
        ret = kLanguagePortuguese;
    }
    else if (0 == strcmp("ar", pLanguageName))
    {
        ret = kLanguageArabic;
    }
    
    return ret;
}

TargetPlatform CCDevice::getTargetPlatform()
{
    return kTargetAndroid;
}

NS_CC_END
