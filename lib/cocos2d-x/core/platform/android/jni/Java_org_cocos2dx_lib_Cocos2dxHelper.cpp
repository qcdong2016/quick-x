#include <stdlib.h>
#include <jni.h>
#include <string>
#include "JniHelper.h"
#include "cocoa/CCString.h"
#include "Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "CCDevice.h"
#include "engine/CCDirector.h"
#include "engine/CCSubSystem.h"
#include "platform/CCInput.h"

#define  LOG_TAG    "Java_org_cocos2dx_lib_Cocos2dxHelper.cpp"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

#define  CLASS_NAME "org/cocos2dx/lib/Cocos2dxHelper"

static EditTextCallback s_pfEditTextCallback = NULL;
static void* s_ctx = NULL;

using namespace cocos2d;
using namespace std;

int getFontSizeAccordingHeightJni(int height) {
    int ret = 0;

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxBitmap", "getFontSizeAccordingHeight", "(I)I")) {
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID, height);
        t.env->DeleteLocalRef(t.classID);
    }

    return ret;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////
void openKeyboardJNI() {
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "openIMEKeyboard", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void closeKeyboardJNI() {
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, "org/cocos2dx/lib/Cocos2dxGLSurfaceView", "closeIMEKeyboard", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

extern "C" {


    
    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesBegin(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y) {
        // cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesBegin(1, &id, &x, &y);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesEnd(JNIEnv * env, jobject thiz, jint id, jfloat x, jfloat y) {
        // cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesEnd(1, &id, &x, &y);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesMove(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        // cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesMove(size, id, x, y);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeTouchesCancel(JNIEnv * env, jobject thiz, jintArray ids, jfloatArray xs, jfloatArray ys) {
        int size = env->GetArrayLength(ids);
        jint id[size];
        jfloat x[size];
        jfloat y[size];

        env->GetIntArrayRegion(ids, 0, size, id);
        env->GetFloatArrayRegion(xs, 0, size, x);
        env->GetFloatArrayRegion(ys, 0, size, y);

        // cocos2d::CCDirector::sharedDirector()->getOpenGLView()->handleTouchesCancel(size, id, x, y);
    }

    #define KEYCODE_BACK 0x04
    #define KEYCODE_MENU 0x52

    JNIEXPORT jboolean JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeKeyDown(JNIEnv * env, jobject thiz, jint keyCode) {
        CCDirector* pDirector = CCDirector::sharedDirector();
        
        if (keyCode == KEYCODE_BACK)
            pDirector->getSubSystem<cocos2d::Input>()->onKeypadBack();
        else if (keyCode == KEYCODE_MENU)
            pDirector->getSubSystem<cocos2d::Input>()->onKeypadMenu();

        return JNI_TRUE;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeRender(JNIEnv* env) {
        cocos2d::CCDirector::sharedDirector()->mainLoop();
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeInsertText(JNIEnv* env, jobject thiz, jstring text) {
        const char* pszText = env->GetStringUTFChars(text, NULL);
        // cocos2d::CCIMEDispatcher::sharedDispatcher()->dispatchInsertText(pszText, strlen(pszText));
        env->ReleaseStringUTFChars(text, pszText);
    }

    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxRenderer_nativeDeleteBackward(JNIEnv* env, jobject thiz) {
        // cocos2d::CCIMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////////


    JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxHelper_nativeSetEditTextDialogResult(JNIEnv * env, jobject obj, jbyteArray text) {
        jsize  size = env->GetArrayLength(text);

        if (size > 0) {
            jbyte * data = (jbyte*)env->GetByteArrayElements(text, 0);
            char* pBuf = (char*)malloc(size+1);
            if (pBuf != NULL) {
                memcpy(pBuf, data, size);
                pBuf[size] = '\0';
                // pass data to edittext's delegate
                if (s_pfEditTextCallback) s_pfEditTextCallback(pBuf, s_ctx);
                free(pBuf);
            }
            env->ReleaseByteArrayElements(text, data, 0);
        } else {
            if (s_pfEditTextCallback) s_pfEditTextCallback("", s_ctx);
        }
    }

}

void showDialogJNI(const char * pszMsg, const char * pszTitle) {
    if (!pszMsg) {
        return;
    }

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showDialog", "(Ljava/lang/String;Ljava/lang/String;)V")) {
        jstring stringArg1;

        if (!pszTitle) {
            stringArg1 = t.env->NewStringUTF("");
        } else {
            stringArg1 = t.env->NewStringUTF(pszTitle);
        }

        jstring stringArg2 = t.env->NewStringUTF(pszMsg);
        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

void releaseEdit(){
    s_pfEditTextCallback = NULL;
    s_ctx = NULL;
}

void showEditTextDialogJNI(const char* pszTitle, const char* pszMessage, int nInputMode, int nInputFlag, int nReturnType, int nMaxLength, EditTextCallback pfEditTextCallback, void* ctx) {
    if (pszMessage == NULL) {
        return;
    }

    s_pfEditTextCallback = pfEditTextCallback;
    s_ctx = ctx;

    JniMethodInfo t;
    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showEditTextDialog", "(Ljava/lang/String;Ljava/lang/String;IIII)V")) {
        jstring stringArg1;

        if (!pszTitle) {
            stringArg1 = t.env->NewStringUTF("");
        } else {
            stringArg1 = t.env->NewStringUTF(pszTitle);
        }

        jstring stringArg2 = t.env->NewStringUTF(pszMessage);

        t.env->CallStaticVoidMethod(t.classID, t.methodID, stringArg1, stringArg2, nInputMode, nInputFlag, nReturnType, nMaxLength);

        t.env->DeleteLocalRef(stringArg1);
        t.env->DeleteLocalRef(stringArg2);
        t.env->DeleteLocalRef(t.classID);
    }
}

std::string getFileDirectoryJNI() {
    JniMethodInfo t;
    std::string ret("");

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCocos2dxWritablePath", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }
    
    return ret;
}

std::string getCurrentLanguageJNI() {
    JniMethodInfo t;
    std::string ret("");

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getCurrentLanguage", "()Ljava/lang/String;")) {
        jstring str = (jstring)t.env->CallStaticObjectMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
        ret = JniHelper::jstring2string(str);
        t.env->DeleteLocalRef(str);
    }

    return ret;
}

void enableAccelerometerJNI() {
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "enableAccelerometer", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

void setAccelerometerIntervalJNI(float interval) {
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "setAccelerometerInterval", "(F)V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID, interval);
        t.env->DeleteLocalRef(t.classID);
    }
}

void disableAccelerometerJNI() {
    JniMethodInfo t;

    if (JniHelper::getStaticMethodInfo(t, CLASS_NAME, "disableAccelerometer", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}
