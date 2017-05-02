
#include "PlayerModule.h"
#include "PlayerUI.h"
#include "CCPlatformMacros.h"
#include "CCPlatformConfig.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"
#include "engine/CCFileSystem.h"
#include "CCKeyCode.h"
#include "CCInput.h"

NS_CC_BEGIN

void PlayerModule::draw(EventData& data)
{
	ImGuiCC::draw();
}

void PlayerModule::update(EventData& data) 
{
	Input* input = CCDirector::sharedDirector()->input;
	if (input->isMouseButtonDown(MOUSEB_LEFT)) {
		ImGuiCC::setMouseDown(true);
	}
	else {
		ImGuiCC::setMouseDown(false);
	}
	ImGuiCC::setMousePos(input->getMousePos().x, input->getMousePos().y);

	if (input->isKeyDown(KEY_F5)) {
		PlayerUI::relunch();
	}
}


PlayerModule::PlayerModule()
{
	ImGuiCC::init();
	ImGuiCC::add<PlayerUI>();
	ImGuiCC::add<LuaUI>();

	subscribeToEvent<AfterDraw>(Handler(this, &PlayerModule::draw));
	subscribeToEvent<UpdateEvent>(Handler(this, &PlayerModule::update));
}

PlayerModule::~PlayerModule()
{
}


NS_CC_END
