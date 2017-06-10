#include "engine/CCDirector.h"
#include "engine/CCMain.h"
#include "audio/SimpleAudioEngine.h"
#include "scripting/CCLuaEngine.h"
#include <string>
#include "CCEGLView.h"
#include "spine/spine-cocos2dx.h"
#include "imgui/PlayerModule.h"

using namespace cocos2d;
class  App : public CCApplication
{
public:
	virtual ~App() { }

	bool applicationDidFinishLaunching()
	{
		CCDirector *d = CCDirector::sharedDirector();

		d->addModule<SpineModule>();
#if BUILD_FOR(MAC) || BUILD_FOR(WIN32)
#endif

		// initialize director
		CCEGLView* view = CCEGLView::sharedOpenGLView();
		view->setFrameSize(CCSize(640, 960));
		d->setOpenGLView(view);
		d->setProjection(kCCDirectorProjection2D);

		// set FPS. the default value is 1.0/60 if you don't call this
		d->setFps(60);

		// register lua engine
		CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
		CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
		pEngine->executeScriptFile("scripts/main.lua");

		return true;
	}

	// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
	void applicationDidEnterBackground()
	{
		CCDirector::sharedDirector()->pause();
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}

	// this function will be called when the app is active again
	void applicationWillEnterForeground()
	{
		CCDirector::sharedDirector()->resume();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	};
};


QUICKX_APP(App)
