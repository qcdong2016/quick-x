
#pragma once

#include "engine/CCSubSystem.h"
#include "cocoa/CCGeometry.h"
#include <set>

NS_CC_BEGIN;


class Input : public SubSystem 
{
	CCOBJECT(Input, SubSystem)

public:
	Input();

	void setAccelerationEnable(bool enable);
	void setAccelerationInterval(float interval);

public:
	void onKeypadBack();
	void onKeypadMenu();

	void onEnterForeground();
	void onEnterBackground();

	void onAcceleration(float x, float y, float z, float timeStamp);

	void update();

	bool isKeyDown(int key);
	bool isMouseButtonDown(int button);
	const CCPoint& getMousePos() { return _mousePos; }

private:
	void handleEvent(void* evt);
	void setKeyDown(bool down, int key);
	void setMouseButton(bool down, int button);

	int _mouseButton;
	CCPoint _mousePos;
	std::set<int> _keyDown;
};

NS_CC_END;