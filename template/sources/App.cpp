#include "engine/CCDirector.h"
#include "audio/SimpleAudioEngine.h"
#include "scripting/CCLuaEngine.h"
#include <string>
#include "CCEGLView.h"

#include "engine/CCModule.h"
#include "spine/spine-cocos2dx.h"
#include "imgui/NoUI.h"

using namespace cocos2d;
class  App : public CCApplication
{
public:
	virtual ~App() {
		SimpleAudioEngine::sharedEngine()->end();
		ModuleManager::shutdown();//re-write
	}

	bool applicationDidFinishLaunching()
	{
		ModuleManager::addModule<SpineModule>();
#if OS_IS(MAC) || OS_IS(WIN)
		ModuleManager::addModule<NoUIModule>();
#endif

		// initialize director
		CCDirector *pDirector = CCDirector::sharedDirector();
		pDirector->setOpenGLView(CCEGLView::sharedOpenGLView());
		pDirector->setProjection(kCCDirectorProjection2D);

		// set FPS. the default value is 1.0/60 if you don't call this
		pDirector->setAnimationInterval(1.0 / 60);

		// register lua engine
		CCLuaEngine *pEngine = CCLuaEngine::defaultEngine();
		CCScriptEngineManager::sharedManager()->setScriptEngine(pEngine);
		pEngine->executeScriptFile("scripts/main.lua");

		return true;
	}

	// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
	void applicationDidEnterBackground()
	{
		CCDirector::sharedDirector()->stopAnimation();
		CCDirector::sharedDirector()->pause();
		SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	}

	// this function will be called when the app is active again
	void applicationWillEnterForeground()
	{
		CCDirector::sharedDirector()->startAnimation();
		CCDirector::sharedDirector()->resume();
		SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
		SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	};
};


QUICKX_APP(App)