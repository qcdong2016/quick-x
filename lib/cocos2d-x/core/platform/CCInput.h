
#pragma once

#include "engine/CCSubSystem.h"
#include "cocoa/CCGeometry.h"
#include <set>
#include <list>
#include <map>

NS_CC_BEGIN;

struct TouchState {
	int touchid;
	CCPoint positon;
	CCPoint lastPosition;
	CCPoint delta;
	float pressure;
};


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

	int getTouchIndexFromID(int touchID);
	int popTouchIndex();
	void pushTouchIndex(int touchID);
	void resetTouches();

	int _mouseButton;
	CCPoint _mousePos;
	std::set<int> _keyDown;
	std::map<int, TouchState> _touches;
	std::map<int, int> _touchIDMap;
	std::list<int> _availableTouchIDs;
};

NS_CC_END;