#include "jni/JniHelper.h"
#include "jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"
#include "CCApplication.h"
#include "engine/CCDirector.h"
#include "CCEGLView.h"
#include <android/log.h>
#include <jni.h>
#include <cstring>

NS_CC_BEGIN

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication()
{
    CCAssert(! sm_pSharedApplication, "");
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CCAssert(this == sm_pSharedApplication, "");
    sm_pSharedApplication = NULL;
}

int CCApplication::run()
{
    // Initialize instance and cocos2d.
    if (! applicationDidFinishLaunching())
    {
        return 0;
    }
    
    return -1;
}

void CCApplication::setAnimationInterval(double interval)
{
    JniMethodInfo methodInfo;
    if (! JniHelper::getStaticMethodInfo(methodInfo, "org/cocos2dx/lib/Cocos2dxRenderer", "setAnimationInterval", 
        "(D)V"))
    {
        CCLOG("%s %d: error to get methodInfo", __FILE__, __LINE__);
    }
    else
    {
        methodInfo.env->CallStaticVoidMethod(methodInfo.classID, methodInfo.methodID, interval);
    }
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication* CCApplication::sharedApplication()
{
    CCAssert(sm_pSharedApplication, "");
    return sm_pSharedApplication;
}


NS_CC_END
