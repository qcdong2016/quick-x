
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
#define IMGUI_DEFINE_MATH_OPERATORS
#include "imgui/imgui_internal.h"
#include "cocoa/TransformUtils.h"
#include "nodes/CCDrawingPrimitives.h"

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
	ImGui::PushID(node);
    CCArray* arr = node->getChildren();

	bool hasChild = (arr && arr->count() != 0);

	if (ImGui::TreeNodeEx(node->getName(), hasChild ? 0 : ImGuiTreeNodeFlags_Leaf))
	{
		if (hasChild)
		{
			for (int i = 0; i < arr->count(); i++)
			{
				DrawNode((CCNode*)arr->objectAtIndex(i));
			}
		}
		ImGui::TreePop();
	}
	ImGui::PopID();
}

void PlayerModule::hierarchyDraw(ImVec2 area)
{
    CCArray* arr = root->getChildren();

	for (int i = 0; i < arr->count(); i++)
	{
		DrawNode((CCNode*)arr->objectAtIndex(i));
	}
}

void PlayerModule::inspectorDraw(ImVec2 area)
{
    
}

static bool testNode(CCNode* node, const Vec2& pos)
{
	Vec2 nsp = node->convertToNodeSpaceAR(pos);
	Vec2 anchor = node->getAnchorPoint();
	CCSize size = node->getSize();

	CCRect bb = CCRect(-size.width * anchor.x, -size.height * anchor.y, size.width, size.height);
	return bb.containsPoint(nsp);
}

static CCNode* checkNode(CCNode* root, const Vec2& pos)
{
	CCArray* children = root->getChildren();

	if (children)
	{
		for (int i = children->count() - 1; i >= 0; i--)
		{
			CCNode* node = checkNode((CCNode*)children->objectAtIndex(i), pos);
			if (node)
				return node;
		}
	}
	if (testNode(root, pos))
		return root;

	return nullptr;
}

static void drawPoint(const Vec2& pos) 
{
	Vec2 p = Vec2(5, 5);
	ccDrawSolidRect(pos - p, pos + p, ccc4f(0.0f, 1.0f, 0.0f, 1.0f));
}

void PlayerModule::drawScene(ImVec2 area)
{
	ImGuiStyle& st = ImGui::GetStyle();

	area.x -= 2 * st.WindowPadding.x;
	area.y -= 2 * st.WindowPadding.y;

	if (!tex || sceneSize.x != area.x || sceneSize.y != area.y) {
		tex.Reset();
		tex = CCRenderTexture::create(area.x, area.y);
		root->setPosition(Vec2(area.x / 2, area.y / 2));
		sceneSize.x = area.x;
		sceneSize.y = area.y;
	}

	ImGui::InvisibleButton("canvas", area);

	tex->beginWithClear(0, 0, 0, 1);
	root->visit();
	if (selected)
	{
		kmGLPushMatrix();

		kmMat4 transfrom4x4;

		CCAffineTransform tmpAffine = selected->nodeToWorldTransform();
		CGAffineToGL(&tmpAffine, transfrom4x4.mat);

		kmGLMultMatrix(&transfrom4x4);

		const CCSize& size = selected->getSize();
		const Vec2& anchor = selected->getAnchorPoint();

		Vec2 origin;// (-size.width * anchor.x, -size.height * anchor.y);
		Vec2 destination(origin + size);
		ccDrawColor4F(0.0f, 1.0f, 0.0f, 1.0f);
		ccDrawRect(origin, destination);

		kmGLPopMatrix();

		drawPoint(CCPointApplyAffineTransform(Vec2(size.width * anchor.x, size.height * anchor.y), tmpAffine));
		drawPoint(CCPointApplyAffineTransform(origin, tmpAffine));
		drawPoint(CCPointApplyAffineTransform(destination, tmpAffine));
		drawPoint(CCPointApplyAffineTransform(Vec2(origin.x, destination.y), tmpAffine));
		drawPoint(CCPointApplyAffineTransform(Vec2(destination.x, origin.y), tmpAffine));
	}

	tex->end();

	CCSprite* sp = tex->getSprite();
	CCTexture2D* tex = sp->getTexture();
	ImTextureID id = (void*)tex->getName();

	ImDrawList* list = ImGui::GetWindowDrawList();
	list->AddImage(id, ImGui::GetItemBoxMin(), ImGui::GetItemBoxMax(), ImVec2(0, 1), ImVec2(1, 0));

	ImVec2 mouse = ImGui::GetMousePos();
	mouse.x = mouse.x - ImGui::GetItemBoxMin().x;
	mouse.y = ImGui::GetItemBoxMax().y - mouse.y;

	static bool isDown = false;
	static ImVec2 last;
	if (ImGui::IsItemClicked(0))
	{
		isDown = true;
		last = ImGui::GetMousePos();
		selected = checkNode(root, Vec2(mouse.x, mouse.y));
	}

	if (!ImGui::IsMouseDown(0))
	{
		isDown = false;
	}

	if (isDown && ImGui::IsItemHovered())
	{
		ImVec2 cur = ImGui::GetMousePos();
		ImVec2 dif = cur - last;
		CCLog("%f %f, %f %f", cur.x, cur.y, last.x, last.y);
		last = cur;
		if (selected)
		{
			selected->setPosition(selected->getPosition() + Vec2(dif.x, -dif.y));
		}
	}
	else
	{
		isDown = false;
	}
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
	root->setName("root");

	CCSprite* sp = CCSprite::create("res/HelloWorld.png");
	sp->setName("sprite");
    root->addChild(sp);

	CCLabelTTF* label = CCLabelTTF::create();
	label->setZOrder(1);
	label->setFontSize(100);
	label->setString("hello world");
	label->setPosition(Vec2(200, 100));
	label->setName("label");
	root->addChild(label);

	{//scale
		CCLabelTTF* label = CCLabelTTF::create();
		label->setZOrder(1);
		label->setFontSize(100);
		label->setString("hello world");
		label->setPosition(Vec2(300, 400));
		label->setName("label2");
		label->setScale(0.5);
		sp->addChild(label);
	}
	{//rotation
		CCLabelTTF* label = CCLabelTTF::create();
		label->setFontSize(100);
		label->setString("hello world");
		label->setPosition(Vec2(200, 100));
		label->setName("label2");
		label->setRotation(60);
		sp->addChild(label);
	}
	{//anchor
		CCLabelTTF* label = CCLabelTTF::create();
		label->setFontSize(100);
		label->setString("hello world");
		label->setPosition(Vec2(200, 100));
		label->setName("label2");
		label->setAnchorPoint(Vec2(0.5, 1));
		sp->addChild(label);
	}
}

PlayerModule::~PlayerModule()
{
}


NS_CC_END
