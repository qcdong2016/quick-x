
#pragma once
#include "engine/CCSubSystem.h"
#include "engine/CCDirector.h"
#include "PlayerUI.h"
#include "ImGuiDock.h"
#include "nodes/CCRenderTexture.h"

NS_CC_BEGIN

class PlayerModule : public Module
{
public:
	PlayerModule();
	~PlayerModule();

	void update(EventData& data);
	void draw(EventData& data);


	void drawScene(ImVec2 area);
    void hierarchyDraw(ImVec2 area);
    void inspectorDraw(ImVec2 area);
private:
	PlayerUI ui;
	ImGuiDock::Dockspace dockspace;
	ImGuiDock::Dock scene;
	ImGuiDock::Dock inspector;
	ImGuiDock::Dock hierarchy;
	ImGuiDock::Dock console;
	ImGuiDock::Dock assets;

	SharedPtr<CCRenderTexture> tex;
	SharedPtr<CCNode> root;
	WeakPtr<CCNode> selected;

	ImVec2 sceneSize;
};

NS_CC_END
