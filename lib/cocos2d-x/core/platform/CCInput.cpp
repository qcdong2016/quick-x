#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"
#include "SDL_events.h"
#include "CCKeyCode.h"

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
#include "CCEGLView.h"
#include "CCKeyCode.h"

NS_CC_BEGIN

static int convertSDLKeyCode(int keySym, int scanCode)
{
	if (scanCode == SCANCODE_AC_BACK)
		return KEY_ESCAPE;
	else
		return SDL_tolower(keySym);
}

Input::Input()
	: _mouseButton(0)
{
}

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

void Input::update()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
		handleEvent(&event);
}

bool Input::isKeyDown(int key)
{
	return _keyDown.count(key) > 0;
}

void Input::setKeyDown(bool down, int key) {

	if (down)
		_keyDown.insert(key);
	else 
		_keyDown.erase(key);
}

bool Input::isMouseButtonDown(int button) {
	return (_mouseButton & button) != 0;
}

void Input::setMouseButton(bool down, int button) {
	if (down) {
		_mouseButton |= button;
	}
	else {
		if (_mouseButton & button)
			_mouseButton &= ~button;
	}
}


void Input::handleEvent(void* evt)
{
	SDL_Event& e = *static_cast<SDL_Event*>(evt);
	const CCSize& size = CCEGLView::sharedOpenGLView()->getFrameSize();

	switch (e.type)
	{
	case SDL_QUIT:
		CCDirector::sharedDirector()->shutdown();
		break;
	case SDL_MOUSEBUTTONDOWN:
	{
		setMouseButton(true, 1 << (e.button.button - 1));

		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Event event;
		event.type = SDL_FINGERDOWN;
		event.tfinger.touchId = 0;
		event.tfinger.fingerId = e.button.button - 1;
		event.tfinger.pressure = 1.0f;
		event.tfinger.x = (float)x / size.width;
		event.tfinger.y = (float)y / size.height;
		event.tfinger.dx = 0;
		event.tfinger.dy = 0;
		SDL_PushEvent(&event);
		break;
	}
	case SDL_MOUSEBUTTONUP:
	{
		setMouseButton(false, 1 << (e.button.button - 1));

		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Event event;
		event.type = SDL_FINGERUP;
		event.tfinger.touchId = 0;
		event.tfinger.fingerId = e.button.button - 1;
		event.tfinger.pressure = 0.0f;
		event.tfinger.x = (float)x / size.width;
		event.tfinger.y = (float)y / size.height;
		event.tfinger.dx = 0;
		event.tfinger.dy = 0;
		SDL_PushEvent(&event);
		break;
	}
	case SDL_MOUSEMOTION:
	{
		_mousePos.x += e.motion.xrel;
		_mousePos.y += e.motion.yrel;

		int x, y;
		SDL_GetMouseState(&x, &y);

		SDL_Event event;
		event.type = SDL_FINGERMOTION;
		event.tfinger.touchId = 0;
		event.tfinger.fingerId = 0;
		event.tfinger.pressure = 1.0f;
		event.tfinger.x = (float)x / size.width;
		event.tfinger.y = (float)y / size.height;
		event.tfinger.dx = (float)e.motion.xrel / size.width;
		event.tfinger.dy = (float)e.motion.yrel / size.height;
		SDL_PushEvent(&event);
		break;
	}
	case SDL_FINGERDOWN:
	{
		CCPoint pt((int)(e.tfinger.x * size.width), (int)(e.tfinger.y * size.height));
		int id = 0;
		CCEGLView::sharedOpenGLView()->handleTouchesBegin(1, &id, &pt.x, &pt.y);
		break;
	}
	case SDL_FINGERUP:
	{
		CCPoint pt((int)(e.tfinger.x * size.width), (int)(e.tfinger.y * size.height));
		int id = 0;
		CCEGLView::sharedOpenGLView()->handleTouchesEnd(1, &id, &pt.x, &pt.y);
		break;
	}
	case SDL_FINGERMOTION:
	{
		CCPoint pt((int)(e.tfinger.x * size.width), (int)(e.tfinger.y * size.height));
		int id = 0;
		CCEGLView::sharedOpenGLView()->handleTouchesMove(1, &id, &pt.x, &pt.y);
		break;
	}
	case SDL_KEYDOWN:
	{
		setKeyDown(true, convertSDLKeyCode(e.key.keysym.sym, e.key.keysym.scancode));
		break;
	}
	case SDL_KEYUP:
	{
		setKeyDown(true, convertSDLKeyCode(e.key.keysym.sym, e.key.keysym.scancode));
		break;
	}
	case SDL_TEXTINPUT:
	{
		break;
	}
	default:
		break;
	}
}

NS_CC_END;

