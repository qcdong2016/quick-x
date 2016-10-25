#include "CCInput.h"
#include "CCDirector.h"

NS_CC_BEGIN;

void Input::onKeypadBack()
{
	VariantMap eventData;
	using namespace KeypadEvent;
	eventData[P_KEY] = "back";
	this->sendEvent(E_KEYPAD, eventData);
}

void Input::onKeypadMenu()
{
	VariantMap eventData;
	using namespace KeypadEvent;
	eventData[P_KEY] = "menu";
	this->sendEvent(E_KEYPAD, eventData);
}

NS_CC_END;
