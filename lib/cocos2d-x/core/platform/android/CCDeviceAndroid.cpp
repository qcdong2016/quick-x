#include "platform/CCDevice.h"
#include "android/jni/JniHelper.h"
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
        JniMethodInfo t;
        if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxHelper", "getDPI", "()I")) {
            dpi = t.env->CallStaticIntMethod(t.classID, t.methodID);
            t.env->DeleteLocalRef(t.classID);
        }
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


//  activity indicator

void CCDevice::showActivityIndicator(void)
{
    CCLOG("CCDevice::showActivityIndicator() - not support this platform.");
}

void CCDevice::hideActivityIndicator(void)
{
    CCLOG("CCDevice::hideActivityIndicator() - not support this platform.");
}


//  alert view

void CCDevice::createAlert(const char* title,
                           const char* message,
                           const char* cancelButtonTitle)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "createAlert", 
        "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/String;)V"))
    {
        jstring jtitle = methodInfo.env->NewStringUTF(title);
        jstring jmessage = methodInfo.env->NewStringUTF(message);
        jstring jcancelButtonTitle = methodInfo.env->NewStringUTF(cancelButtonTitle);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jtitle, jmessage, jcancelButtonTitle);
        methodInfo.env->DeleteLocalRef(jtitle);
        methodInfo.env->DeleteLocalRef(jmessage);
        methodInfo.env->DeleteLocalRef(jcancelButtonTitle);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

int CCDevice::addAlertButton(const char* buttonTitle)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "addAlertButton", 
        "(Ljava/lang/String;)I"))
    {
        jstring jbuttonTitle = methodInfo.env->NewStringUTF(buttonTitle);
        jint ret = methodInfo.env->CallStaticIntMethod(methodInfo.classID, methodInfo.methodID, jbuttonTitle);
        methodInfo.env->DeleteLocalRef(jbuttonTitle);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        
        return ret;
    }
    return 0;
}

void CCDevice::showAlert(CCAlertViewDelegate* delegate)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "showAlert", 
        "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

void CCDevice::cancelAlert(void)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "cancelAlert", 
        "()V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}


//  misc

void CCDevice::openURL(const char* url)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "openURL", 
        "(Ljava/lang/String;)V"))
    {
        jstring jurl = methodInfo.env->NewStringUTF(url);
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, jurl);
        methodInfo.env->DeleteLocalRef(jurl);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}

const std::string CCDevice::getInputText(const char* title, const char* message, const char* defaultValue)
{
    CCLOG("CCDevice::getInputText() - not support this platform.");
    return std::string("");
}


//  OpenUDID

const std::string CCDevice::getOpenUDID(void)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "getOpenUDID", 
        "()Ljava/lang/String;"))
    {
        jstring judid = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        char* udid = (char*)methodInfo.env->GetStringUTFChars(judid, 0);
		std::string ret = udid;
        methodInfo.env->ReleaseStringUTFChars(judid, udid);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return ret;
    }
    return std::string("");
}

const std::string CCDevice::getDeviceName(void)
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "getDeviceName", 
        "()Ljava/lang/String;"))
    {
        jstring jdevice = (jstring)methodInfo.env->CallStaticObjectMethod(methodInfo.classID, methodInfo.methodID);
        char* device = (char*)methodInfo.env->GetStringUTFChars(jdevice, 0);
		std::string ret = device;
        methodInfo.env->ReleaseStringUTFChars(jdevice, device);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
        return ret;
    }
    return std::string("");
}

void CCDevice::vibrate()
{
    JniMethodInfo methodInfo;
    if (JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/utils/PSNative", "vibrate", 
        "(I)V"))
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, 200);
        methodInfo.env->DeleteLocalRef(methodInfo.classID);
    }
}


NS_CC_END
