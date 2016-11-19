#include "CCInput.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"

NS_CC_BEGIN;

void Input::onKeypadBack()
{
	EventDataMap eventData;
	eventData[KeypadEvent::Param::key] = "back";
	sendEvent<KeypadEvent::Param>(eventData);
}

void Input::onKeypadMenu()
{
	EventDataMap eventData;
	eventData[KeypadEvent::Param::key] = "menu";
	sendEvent<KeypadEvent::Param>(eventData);
}

void Input::onEnterBackground()
{
	EventDataMap eventData;
	sendEvent<EnterBackground::Param>(eventData);
}

void Input::onEnterForeground()
{
	EventDataMap eventData;
	sendEvent<EnterForeground::Param>(eventData);
}

NS_CC_END;

