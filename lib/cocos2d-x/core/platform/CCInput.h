
#pragma once

#include "engine/CCSubSystem.h"
#include "cocoa/CCGeometry.h"
#include <set>
#include <list>
#include <map>
#include <functional>

NS_CC_BEGIN;

struct EventCallFuncData
{
    std::function<void()> func;
};

enum UserEventType
{
    CALL_FUNC
};

struct TouchState {
	int touchid;
	Vec2 positon;
	Vec2 lastPosition;
	Vec2 delta;
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
	const Vec2& getMousePos() { return _mousePos; }
    
    // thread safe
    bool pushUserEvent(int code, void* data);

private:
	void handleEvent(void* evt);
	void setKeyDown(bool down, int key);
	void setMouseButton(bool down, int button);

	int getTouchIndexFromID(int touchID);
	int popTouchIndex();
	void pushTouchIndex(int touchID);
	void resetTouches();

	int _mouseButton;
	Vec2 _mousePos;
	std::set<int> _keyDown;
	std::map<int, TouchState> _touches;
	std::map<int, int> _touchIDMap;
	std::list<int> _availableTouchIDs;
};

NS_CC_END;
