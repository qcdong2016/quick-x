#ifndef _PLAYER_UI_
#define _PLAYER_UI_

#include "imgui_cocos2dx.h"
#include <vector>
#include "LuaFunction.h"

NS_CC_BEGIN

class PlayerUI : public UIInterface
{
public:

	virtual void draw();
	static const char* name() { return "PlayerUI"; }

public:

	PlayerUI();

	static void relunch();
	void setFrameSize(int w, int h);
	void setZoom(float zoom);

};

class LuaUI : public UIInterface
{
public:
	virtual void draw();
	static const char* name() { return "LuaUI"; }

	void addFunction(SharedPtr<LuaFunction>& func) {
		_functions.push_back(func);
	}

private:
	std::vector<SharedPtr<LuaFunction> > _functions;
};

NS_CC_END


#endif//_PLAYER_UI_