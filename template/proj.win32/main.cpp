

#include "..\cocos\ProcessUtils.h"
#include "..\imgui\imgui_cocos2dx.h"
#include "..\imgui\PlayerUI.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include "CCFileUtils.h"

#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

	AppDelegate app;

	cocos2d::parseArguments(GetCommandLineW());
	cocos2d::openConsoleWindow();

	int width = cocos2d::getIntArgument("w");
	int height = cocos2d::getIntArgument("h");
	float zoom = cocos2d::getFloatArgument("zoom");
	std::string workdir = cocos2d::getArgument("workdir");

	if (workdir != "")
		cocos2d::CCFileUtils::sharedFileUtils()->setSearchRootPath(workdir.c_str());

	cocos2d::CCEGLView* eglView = cocos2d::CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(width, height);
	eglView->setFrameZoomFactor(zoom);

	cocos2d::ImGuiCC::setUIInterface<cocos2d::PlayerUI>();

	int ret = app.run();

	return ret;
}
