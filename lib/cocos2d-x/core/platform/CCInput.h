
#pragma once

#include "SubSystem.h"

CC_EVENT(E_KEYPAD, KeypadEvent)
{
	CC_PARAM(P_KEY, key);
};

CC_EVENT(E_UPDATE, UpdateEvent)
{
	CC_PARAM(P_TIMESTEP, timeStep);
};

NS_CC_BEGIN;


class Input : public SubSystem
{
	CCOBJECT(Input, SubSystem)
public:

	void onKeypadBack();
	void onKeypadMenu();

};

NS_CC_END;