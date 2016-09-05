#ifndef _IMGUI_COCOS2DX_H_
#define _IMGUI_COCOS2DX_H_


#include "CCPlatformMacros.h"

NS_CC_BEGIN;

class UIInterface
{
public:
	virtual void draw() = 0;
};


class ImGuiCC
{
public:
	static void init();
	static void draw();
	
	static void setMouseDown(bool down);
	static bool setMousePos(int x, int y);

	static void setKeyDown(int k);
	static void setKeyUp(int k);
	static void setChar(int c);

	static void toggleVisible();
	static void setVisible(bool v);

	template<typename T>
	static void setUIInterface()
	{
		CC_SAFE_DELETE(_interface);
		_interface = new T;
	}

	static UIInterface* _interface;
};

NS_CC_END;

#endif // !_IMGUI_COCOS2DX_H_


