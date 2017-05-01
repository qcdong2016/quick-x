

#include "base/ProcessUtils.h"
#include "engine/CCFileSystem.h"
#include "AppDelegate.h"
#include "CCEGLView.h"
#include <tchar.h>


int quickx_main(int argc, char** argv) {
	USING_NS_CC;

	AppDelegate app;

	parseArguments(GetCommandLineW());
	openConsoleWindow();

	int width = getIntArgument("w");
	int height = getIntArgument("h");
	float zoom = getFloatArgument("zoom");
	std::string workdir = getArgument("workdir");

	FileSystem::setResourceRoot(workdir);

	int ret = app.run();
	CCEGLView* eglView = CCEGLView::sharedOpenGLView();
	eglView->setFrameSize(width, height);
	return ret;
}
