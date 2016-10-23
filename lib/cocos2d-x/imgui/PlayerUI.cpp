#include "imgui/imgui.h"
#include "PlayerUI.h"
#include "base/ProcessUtils.h"
#include "CCEGLView.h"

#define _count_of(_ARR)      ((int)(sizeof(_ARR)/sizeof(*_ARR)))

NS_CC_BEGIN

struct ScreenSize
{
	const char* name;
	int height, width;
};

static ScreenSize AllSize[] = {
    { "iPhone 3Gs (320x480)",    320,  480},
    { "iPhone 4 (640x960)",      640,  960},
    { "iPhone 5 (640x1136)",     640,  1136},
    { "iPad (768x1024)",         768,  1024},
    { "iPad Retina (1536x2048)", 1536, 2048},
    { "Android (480x800)",       480,  800},
    { "Android (480x854)",       480,  854},
    { "Android (540x960)",       540,  960},
    { "Android (600x1024)",      600,  1024},
    { "Android (720x1280)",      720,  1280},
    { "Android (800x1280)",      800,  1280},
    { "Android (1080x1920)",     1080, 1920},
};

bool items_getter(void* allsize, int index, const char** out)
{
	*out = AllSize[index].name;
	return true;
}


static int _landscape;
static int _zoom;

static void changeFrameOrientation()
{
	int w = getIntArgument("w");
	int h = getIntArgument("h");
	setArgument("w", h);
	setArgument("h", w);
}

void PlayerUI::draw()
{
	ImGuiIO& io = ImGui::GetIO();

	ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoTitleBar |
		ImGuiWindowFlags_NoResize |
		ImGuiWindowFlags_NoMove |
		ImGuiWindowFlags_NoScrollbar |
		ImGuiWindowFlags_MenuBar |
		ImGuiWindowFlags_NoCollapse;

	ImGui::SetNextWindowSize(ImVec2(io.DisplaySize.x, 19));
	ImGui::SetNextWindowPos(ImVec2(0, 0));
	ImGui::Begin("Menu", NULL, window_flags);

	bool open_project = false;

	if (ImGui::BeginMenuBar()) {
		if (ImGui::BeginMenu("File")) {
			if (ImGui::MenuItem("Relunch", "F5")) relunch();
			if (ImGui::MenuItem("OpenProject"))
				open_project = true;

			ImGui::EndMenu();
		}
		if (ImGui::BeginMenu("View")) {

			for (int i = 0; i < _count_of(AllSize); i++) {
				ScreenSize& sz = AllSize[i];

				if (ImGui::MenuItem(sz.name))
					setFrameSize(sz.width, sz.height);
			}

			ImGui::Separator();
			if (ImGui::RadioButton("Portrait", &_landscape, 0)) {
				changeFrameOrientation();
				relunch();
			}
			ImGui::SameLine();
			if (ImGui::RadioButton("Landscape", &_landscape, 1)) {
				changeFrameOrientation();
				relunch();
			}
			ImGui::Separator();

			if (ImGui::RadioButton("Zoom(100%)", &_zoom, 0)) setZoom(1);
			ImGui::SameLine();
			if (ImGui::RadioButton("Zoom(50%)", &_zoom, 1)) setZoom(0.5f);

			ImGui::EndMenu();
		}
		ImGui::EndMenuBar();
	}
	ImGui::End();

	if (open_project) {
		ImGui::OpenPopup("OpenProject");
		ImGui::SetNextWindowSize(ImVec2(500, 300));
	}
	static char str0[128] = "Hello, world!";
	static int screenSize = 1;

	if (ImGui::BeginPopupModal("OpenProject"))
	{

		ImGui::BeginGroup();

			ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetItemsLineHeightWithSpacing())); // Leave room for 1 line below us

				ImGui::Text("Choose Project Directory");
				ImGui::InputText("", str0, _count_of(str0));
				ImGui::SameLine();
				if (ImGui::Button("Browse...")) { ImGui::CloseCurrentPopup(); }

				ImGui::Combo("Screen Size", &screenSize, items_getter, AllSize, _count_of(AllSize));

				ImGui::RadioButton("Portrait", &_landscape, 0);
				ImGui::SameLine();
				ImGui::RadioButton("Landscape", &_landscape, 1);

			ImGui::EndChild();
			ImGui::BeginChild("buttons");

			ImVec2 sz = ImVec2(120, 0);
			if (ImGui::Button("OK", sz)) { 
				cocos2d::setArgument("workdir", str0);

				int width = AllSize[screenSize].width;
				int height = AllSize[screenSize].height;

				if (_landscape == 0)
					std::swap(width, height);

				cocos2d::setArgument("w", width);
				cocos2d::setArgument("h", height);

				relunch();
			}
			ImGui::SameLine(); ImGui::Dummy(sz); ImGui::SameLine();
			if (ImGui::Button("Cancel", sz)) { ImGui::CloseCurrentPopup(); }
			ImGui::EndChild();

		ImGui::EndGroup();

		ImGui::EndPopup();
	}
}

PlayerUI::PlayerUI()
{
	int w = getIntArgument("w");
	int h = getIntArgument("h");

	_landscape = (w > h) ? 1 : 0;
	_zoom = (getFloatArgument("zoom", 1) == 1) ? 0 : 1;
}

void PlayerUI::setFrameSize(int w, int h)
{
	setArgument("w", w);
	setArgument("h", h);
	relunch();
}

void PlayerUI::setZoom(float zoom)
{
	CCEGLView* v = CCEGLView::sharedOpenGLView();
//	v->setFrameZoomFactor(zoom);
//	v->resize(v->getFrameSize().width * zoom, v->getFrameSize().height * zoom);

	setArgument("zoom", zoom);
}

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <windows.h>
#include <winbase.h>
#include <libloaderapi.h>
#include <processthreadsapi.h>
#endif

void PlayerUI::relunch()
{

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	TCHAR moduleName[MAX_PATH];
	ZeroMemory(moduleName, sizeof(moduleName));
	GetModuleFileName(NULL, moduleName, MAX_PATH);

	const std::string& commandLine = getArgumentsString();
	std::wstring ws;
	ws.append(L"\"");
	ws.append(moduleName);
	ws.append(L"\" ");
	ws.append(commandLine.begin(), commandLine.end());

	STARTUPINFO si = { 0 };
	PROCESS_INFORMATION pi = { 0 };
	lstrcpyW(moduleName, ws.c_str());
	if (CreateProcess(NULL, moduleName, NULL, NULL, FALSE, NORMAL_PRIORITY_CLASS, NULL, NULL, &si, &pi))
	{
		ExitProcess(0);
	}
#endif
}

void LuaUI::draw()
{
	for (auto& f : _functions) {
		f->BeginCall();
		f->EndCall();
	}
}

NS_CC_END 



