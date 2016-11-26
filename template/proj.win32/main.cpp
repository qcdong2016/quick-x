

#include "base/ProcessUtils.h"
#include "engine/CCFileSystem.h"
#include "AppDelegate.h"
#include "CCEGLView.h"

#include <tchar.h>

int APIENTRY _tWinMain(HINSTANCE hInstance,
    HINSTANCE hPrevInstance,
    LPTSTR    lpCmdLine,
    int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);


	USING_NS_CC;

	AppDelegate app;

	parseArguments(GetCommandLineW());
	openConsoleWindow();

	int width = getIntArgument("w");
	int height = getIntArgument("h");
	float zoom = getFloatArgument("zoom");
	std::string workdir = getArgument("workdir");

	FileSystem::setResourceRoot(workdir);

	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(width, height);
	eglView->setFrameZoomFactor(zoom);

	int ret = app.run();

	return ret;
}
