
#include "PlayerModule.h"
#include "PlayerUI.h"
#include "CCPlatformMacros.h"
#include "CCPlatformConfig.h"
#include "engine/CCDirector.h"
#include "engine/CCEngineEvents.h"
#include "engine/CCFileSystem.h"
#include "CCKeyCode.h"
#include "CCInput.h"
#include "CCEGLView.h"
#include "nodes/CCLabelTTF.h"
#include "nodes/CCScene.h"
#include "kazmath/matrix.h"

NS_CC_BEGIN

void PlayerModule::draw(EventData& data)
{
	
	//glUseProgram(0);
	ImGuiIO& io = ImGui::GetIO();
	io.DeltaTime = CCDirector::sharedDirector()->getDeltaTime();

	// Setup display size (every frame to accommodate for window resizing)
	CCSize sz = CCEGLView::sharedOpenGLView()->getFrameSize();
	int w = sz.width;
	int h = sz.height;
	int display_w = w, display_h = h;

	io.DisplaySize = ImVec2((float)w, (float)h);
	io.DisplayFramebufferScale = ImVec2(w > 0 ? ((float)display_w / w) : 0, h > 0 ? ((float)display_h / h) : 0);

	// Start the frame
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::SetNextWindowSize(ImVec2(sz.width, sz.height));
	ImGui::Begin("dock", nullptr, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	dockspace.updateAndDraw(ImVec2(sz.width, sz.height));
	ImGui::End();

	ImGui::Render();
	
}

void PlayerModule::update(EventData& data) 
{
	ImGuiIO& io = ImGui::GetIO();

	Input* input = CCDirector::sharedDirector()->input;
	if (input->isMouseButtonDown(MOUSEB_LEFT)) {
		io.MouseDown[0] = true;
	}
	else {
		io.MouseDown[0] = false;
	}

	io.DeltaTime = CCDirector::sharedDirector()->getDeltaTime();
	io.MousePos = ImVec2(input->getMousePos().x, input->getMousePos().y);

	if (input->isKeyDown(KEY_F5)) {
		PlayerUI::relunch();
	}
}

static void DrawNode(CCNode* node)
{
    CCArray* arr = node->getChildren();
    if (!arr)
        return;
    
    for (int i = 0; i < arr->count(); i++)
    {
        CCNode* child = (CCNode*)arr->objectAtIndex(i);
        if (ImGui::TreeNode(node->getName()))
        {
            ImGui::TreePop();
        }
        DrawNode(child);
    }
}

void PlayerModule::hierarchyDraw(ImVec2 area)
{
    if (!root)
        return;
   
    CCArray* arr = root->getChildren();
    if (!arr)
        return;
    
    for (int i = 0; i < arr->count(); i++)
    {
        CCNode* node = (CCNode*)arr->objectAtIndex(i);
        if (ImGui::TreeNode(node->getName()))
        {
            ImGui::TreePop();
        }
        DrawNode(node);
    }
}

void PlayerModule::inspectorDraw(ImVec2 area)
{
    
}

void PlayerModule::drawScene(ImVec2 area)
{
	ImGuiStyle& st = ImGui::GetStyle();

	area.x -= 2*st.WindowPadding.x;
	area.y -= 2*st.WindowPadding.y;

	if (!tex || sceneSize.x != area.x || sceneSize.y != area.y) {
		tex.Reset();
		tex = CCRenderTexture::create(area.x, area.y);
		root->setPosition(Vec2(area.x/2, area.y/2));
		sceneSize.x = area.x;
		sceneSize.y = area.y;
	}

	ImGui::InvisibleButton("canvas", area);

	tex->beginWithClear(0, 0, 0, 1);
	root->visit();
	tex->end();

	CCSprite* sp = tex->getSprite();
	CCTexture2D* tex = sp->getTexture();
	ImTextureID id = (void*)tex->getName();

	ImDrawList* list = ImGui::GetWindowDrawList();
	list->AddImage(id, ImGui::GetItemBoxMin(), ImGui::GetItemBoxMax(), ImVec2(0, 1), ImVec2(1, 0));
}

PlayerModule::PlayerModule()
{
	ImGuiCC::init();
	ImGuiCC::add<PlayerUI>();
	ImGuiCC::add<LuaUI>();

	subscribeToEvent<AfterDraw>(Handler(this, &PlayerModule::draw));
	subscribeToEvent<UpdateEvent>(Handler(this, &PlayerModule::update));

	scene.initialize("scene", true, ImVec2(250, 300), std::bind(&PlayerModule::drawScene, this, std::placeholders::_1));
	inspector.initialize("inspector", true, ImVec2(250, 300), std::bind(&PlayerModule::inspectorDraw, this, std::placeholders::_1));
	hierarchy.initialize("hierarchy", true, ImVec2(250, 300), std::bind(&PlayerModule::hierarchyDraw, this, std::placeholders::_1));
	console.initialize("console", true, ImVec2(250, 300), [](ImVec2 area) { ImGui::Text("Hello :)"); });
	assets.initialize("assets", true, ImVec2(250, 300), [](ImVec2 area) { ImGui::Text("Hello :)"); });

	dockspace.dock(&scene, ImGuiDock::DockSlot::None, 200, true);
	dockspace.dockWith(&inspector, &scene, ImGuiDock::DockSlot::Right, 300, true);
	dockspace.dockWith(&hierarchy, &scene, ImGuiDock::DockSlot::Left, 300, true);
	dockspace.dock(&console, ImGuiDock::DockSlot::Bottom, 250, true);
	dockspace.dockWith(&assets, &console, ImGuiDock::DockSlot::Tab, 250, true);

    FileSystem::addResourcePath("res/");
    root = CCNode::create();
	CCSprite* sp = CCSprite::create("res/HelloWorld.png");
    root->addChild(sp);

	CCLabelTTF* label = CCLabelTTF::create();
	label->setFontSize(100);
	label->setString("hello world");
	root->addChild(label);
}

PlayerModule::~PlayerModule()
{
}


NS_CC_END
