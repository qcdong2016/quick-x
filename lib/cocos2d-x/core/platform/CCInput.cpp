#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"

NS_CC_BEGIN;

void Input::onKeypadBack()
{
	EventData<KeypadEvent> eventData;
	eventData[KeypadEvent::key] = "back";
	sendEvent<KeypadEvent>(eventData);
}

void Input::onKeypadMenu()
{
	EventData<KeypadEvent> eventData;
	eventData[KeypadEvent::key] = "menu";
	sendEvent<KeypadEvent>(eventData);
}

void Input::onEnterBackground()
{
	EventData<EnterBackground> eventData;
	sendEvent<EnterBackground>(eventData);
}

void Input::onEnterForeground()
{
	EventData<EnterForeground> eventData;
	sendEvent<EnterForeground>(eventData);
}

NS_CC_END;

