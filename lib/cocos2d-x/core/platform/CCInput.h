
#pragma once

#include "engine/CCSubSystem.h"



NS_CC_BEGIN;


class Input : public SubSystem
{
	CCOBJECT(Input, SubSystem)

public:

	void setAccelerationEnable(bool enable);
	void setAccelerationInterval(float interval);

public:
	void onKeypadBack();
	void onKeypadMenu();

	void onEnterForeground();
	void onEnterBackground();

	void onAcceleration(float x, float y, float z, float timeStamp);
};

NS_CC_END;