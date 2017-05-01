#include "CCApplication.h"
#include "CCEGLView.h"
#include "engine/CCDirector.h"
#include <algorithm>

#include "SDL_main.h"
#include "platform/CCTimer.h"
#include "engine/CCSubSystem.h"
#include "CCInput.h"

int quickx_main(int argc, char** argv);
int main(int argc, char** argv) {
	return quickx_main(argc, argv);
}

NS_CC_BEGIN

// sharedApplication pointer
CCApplication * CCApplication::sm_pSharedApplication = 0;

CCApplication::CCApplication()
{

    CC_ASSERT(! sm_pSharedApplication);
    sm_pSharedApplication = this;
}

CCApplication::~CCApplication()
{
    CC_ASSERT(this == sm_pSharedApplication);
    sm_pSharedApplication = NULL;
}

int CCApplication::run()
{
	/*
	std::string args;
	for (int i = 1; i < argc; ++i)
	{
		args.append((const char*)argv[i]);
		args.append(" ");
	}

	parseArguments(args);
*/
	CCEGLView* e = CCEGLView::sharedOpenGLView();

	e->createWithSize(640, 960);

	CCDirector* d = CCDirector::sharedDirector();

	d->setOpenGLView(e);

	if (!applicationDidFinishLaunching())
		return 1;

	while (isRunning())
		runFrame();


	return 0;
}
bool CCApplication::isRunning()
{
	return _running;
}
void CCApplication::runFrame()
{
	CCDirector::sharedDirector()->mainLoop();

	SubSystem::get<Input>()->update();
	//_input->update();

	timeLimit();
}

void CCApplication::timeLimit()
{
	long long targetMax = 1000000LL / _fps;
	long long elapsed = 0;

	for (;;)
	{
		elapsed = _frameTimer.elapsed();
		if (elapsed >= targetMax)
			break;

		// Sleep if 1 ms or more off the frame limiting goal
		if (targetMax - elapsed >= 1000LL)
		{
			unsigned sleepTime = (unsigned)((targetMax - elapsed) / 1000LL);
			CCTime::sleep(sleepTime);
		}
	}
	_frameTimer.reset();
}

void CCApplication::setFps(int fps)
{
	_fps = fps;
}

void CCApplication::setAnimationInterval(double interval)
{
	setFps(1.0 / interval);
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
CCApplication* CCApplication::sharedApplication()
{
    CC_ASSERT(sm_pSharedApplication);
    return sm_pSharedApplication;
}

NS_CC_END
