
#pragma once
#include "engine/CCSubSystem.h"
#include "engine/CCDirector.h"
#include "PlayerUI.h"

NS_CC_BEGIN

class PlayerModule : public Module
{
public:
	PlayerModule();
	~PlayerModule();

	void update(EventData& data);
	void draw(EventData& data);

private:
	PlayerUI ui;
};

NS_CC_END