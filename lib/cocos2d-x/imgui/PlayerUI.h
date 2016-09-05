#ifndef _PLAYER_UI_
#define _PLAYER_UI_

#include "imgui_cocos2dx.h"

NS_CC_BEGIN

class PlayerUI : public UIInterface
{
public:

	virtual void draw();

public:

	PlayerUI();

	static void relunch();
	void setFrameSize(int w, int h);
	void setZoom(float zoom);
};

NS_CC_END


#endif//_PLAYER_UI_