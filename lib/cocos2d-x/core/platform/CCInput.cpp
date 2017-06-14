#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"
#include "SDL_events.h"
#include "CCKeyCode.h"
#include "CCInputEvent.h"

#include "CCEGLView.h"

const unsigned TOUCHID_MAX = 32;

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include <jni.h>
#include "android/jni/JniHelper.h"
#include "android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.h"

extern "C" {

    
	JNIEXPORT void JNICALL Java_org_cocos2dx_lib_Cocos2dxAccelerometer_onSensorChanged(JNIEnv*  env, jobject thiz, jfloat x, jfloat y, jfloat z, jfloat timeStamp) {
		// SubSystem::get<Input>()->onAcceleration(x, y, z, timeStamp);
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

int Input::getTouchIndexFromID(int touchID) {
	const auto& i = _touchIDMap.find(touchID);
	if (i != _touchIDMap.end())
		return i->second;
	int index = popTouchIndex();
	_touchIDMap[touchID] = index;
	return index;
}

int Input::popTouchIndex() {
	if (_availableTouchIDs.empty())
		return 0;
	int index = _availableTouchIDs.front();
	_availableTouchIDs.pop_front();
	return index;
}

void Input::pushTouchIndex(int touchID) {
	const auto& i = _touchIDMap.find(touchID);
	if (i == _touchIDMap.end())
		return;
	int index = _touchIDMap[touchID];
	_touchIDMap.erase(touchID);
	bool inserted = false;
	for (auto i = _availableTouchIDs.begin(); i != _availableTouchIDs.end(); i++) {
		if (*i == index) {
			inserted = true;
			break;
		}

		if (*i > index) {
			_availableTouchIDs.insert(i, index);
			inserted = true;
			break;
		}
	}
	if (inserted)
		_availableTouchIDs.push_back(index);
}

void Input::resetTouches()
{
	_touches.clear();
	_touchIDMap.clear();
	_availableTouchIDs.clear();
	for (int i = 0; i < TOUCHID_MAX; i++)
		_availableTouchIDs.push_back(i);
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
		if (e.tfinger.touchId != SDL_TOUCH_MOUSEID) {
			int touchID = getTouchIndexFromID(e.tfinger.fingerId & 0x7ffffff);
			
			TouchState& state = _touches[touchID];
			state.touchid = touchID;
			state.lastPosition = CCDirector::sharedDirector()->convertToGL(Vec2((int)(e.tfinger.x * size.width), (int)(e.tfinger.y * size.height)));
			state.positon = state.lastPosition;
			state.delta = CCPointZero;
			state.pressure = e.tfinger.pressure;

			TouchBegin eventData;
			eventData[TouchBegin::touchID] = state.touchid;
			eventData[TouchBegin::x] = state.positon.x;
			eventData[TouchBegin::y] = state.positon.y;
			eventData[TouchBegin::pressure] = state.pressure;
			sendEvent(eventData);
		}
		break;
	}
	case SDL_FINGERUP:
	{
		if (e.tfinger.touchId != SDL_TOUCH_MOUSEID) {
			int touchID = getTouchIndexFromID(e.tfinger.fingerId & 0x7ffffff);

			TouchState& state = _touches[touchID];
			
			pushTouchIndex(e.tfinger.fingerId & 0x7ffffff);

			TouchEnd eventData;
			eventData[TouchEnd::touchID] = state.touchid;
			eventData[TouchEnd::x] = state.positon.x;
			eventData[TouchEnd::y] = state.positon.y;
			sendEvent(eventData);

			_touches.erase(touchID);
		}
		break;
	}
	case SDL_FINGERMOTION:
	{
		if (e.tfinger.touchId != SDL_TOUCH_MOUSEID) {
			int touchID = getTouchIndexFromID(e.tfinger.fingerId & 0x7ffffff);

			TouchState& state = _touches[touchID];
			state.touchid = touchID;
			state.positon = CCDirector::sharedDirector()->convertToGL(Vec2((int)(e.tfinger.x * size.width), (int)(e.tfinger.y * size.height)));
			state.delta = state.positon - state.lastPosition;
			state.pressure = e.tfinger.pressure;

			TouchMove eventData;
			eventData[TouchMove::touchID] = state.touchid;
			eventData[TouchMove::x] = state.positon.x;
			eventData[TouchMove::y] = state.positon.y;
			eventData[TouchMove::dx] = e.tfinger.dx * size.width;
			eventData[TouchMove::dy] = e.tfinger.dy * size.height;
			sendEvent(eventData);
		}
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
        TextInput eventData;
        eventData[TextInput::text] = &e.text.text[0];
        sendEvent(eventData);
		break;
	}
    case SDL_TEXTEDITING:
    {
        TextEditing eventData;
        eventData[TextEditing::composition] = &e.edit.text[0];
        eventData[TextEditing::cursor] = e.edit.start;
        eventData[TextEditing::selectionLength] = e.edit.length;
        sendEvent(eventData);
        break;
    }
    case SDL_WINDOWEVENT:
    {
        switch (e.window.event)
        {
        case SDL_WINDOWEVENT_RESIZED:
            CCEGLView::sharedOpenGLView()->onWindowResized();
            break;
        case SDL_WINDOWEVENT_MOVED:
            break;
        }
        break;
    }
        case SDL_USEREVENT:
        {
            switch (e.user.code)
            {
                case CALL_FUNC:
                {
                    ((EventCallFuncData*)e.user.data1)->func();
                    break;
                };
            }
            CC_SAFE_FREE(e.user.data1);
        }
	default:
		break;
	}
}

bool Input::pushUserEvent(int code, void* data)
{
    SDL_Event event;
    SDL_memset(&event, 0, sizeof(event)); /* or SDL_zero(event) */
    event.type = SDL_USEREVENT;
    event.user.code = code;
    event.user.data1 = data;
    event.user.data2 = 0;
    return !!SDL_PushEvent(&event);
}

NS_CC_END
#include "engine/CCEventImpl.h"
#include "CCInputEvent.h"
