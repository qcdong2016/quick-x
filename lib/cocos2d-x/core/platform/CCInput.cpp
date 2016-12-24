#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "android/jni/JniHelper.h"
#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

extern "C" {

	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxAccelerometer_onSensorChanged(JNIEnv*  env, jobject thiz, jfloat x, jfloat y, jfloat z, jfloat timeStamp) {
		SubSystem::get<Input>()->onAcceleration(x, y, z, timeStamp);
	}
}

NS_CC_BEGIN;

void Input::setAccelerationEnable(bool enable)
{
	if (enable)
		enableAccelerometerJNI();
	else
		disableAccelerometerJNI();
}

void Input::setAccelerationInterval(float interval)
{
	setAccelerometerIntervalJNI(interval);
}

NS_CC_END;


#elif CC_TARGET_PLATFORM != CC_PLATFORM_IOS

NS_CC_BEGIN;

void Input::setAccelerationEnable(bool enable)
{
	
}

void Input::setAccelerationInterval(float interval)
{
}

NS_CC_END;

#endif

NS_CC_BEGIN;

void Input::onKeypadBack()
{
	KeypadEvent eventData;
	eventData[KeypadEvent::key] = "back";
	sendEvent(eventData);
}

void Input::onKeypadMenu()
{
	KeypadEvent eventData;
	eventData[KeypadEvent::key] = "menu";
	sendEvent(eventData);
}

void Input::onEnterBackground()
{
	EnterBackground eventData;
	sendEvent(eventData);
}

void Input::onEnterForeground()
{
	EnterForeground eventData;
	sendEvent(eventData);
}

void Input::onAcceleration(float x, float y, float z, float timeStamp)
{
	AccelerationEvent eventData;
	eventData[AccelerationEvent::x] = x;
	eventData[AccelerationEvent::y] = y;
	eventData[AccelerationEvent::z] = z;
	eventData[AccelerationEvent::timeStamp] = timeStamp;
	sendEvent(eventData);
}

NS_CC_END;

