#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"

NS_CC_BEGIN;

void Input::onKeypadBack()
{
	KeypadEvent eventData;
	eventData[KeypadEvent::key] = "back";
	sendEvent<KeypadEvent>(eventData);
}

void Input::onKeypadMenu()
{
	KeypadEvent eventData;
	eventData[KeypadEvent::key] = "menu";
	sendEvent<KeypadEvent>(eventData);
}

void Input::onEnterBackground()
{
	EnterBackground eventData;
	sendEvent<EnterBackground>(eventData);
}

void Input::onEnterForeground()
{
	EnterForeground eventData;
	sendEvent<EnterForeground>(eventData);
}

NS_CC_END;

