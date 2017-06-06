#include "CCEGLView.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include "GL/glew.h"
#endif

#include "SDL.h"
#include "ccMacros.h"
#include "CCGL.h"


NS_CC_BEGIN

SDL_Window* _window = nullptr;
static CCEGLView* _instance = nullptr;

CCEGLView::CCEGLView()
	: _windowSize(960, 640)
    , _glSize(960, 640)
{
}

CCEGLView::~CCEGLView()
{
}

const CCSize& CCEGLView::getDrawableSize()
{
    return _glSize;
}

const CCSize& CCEGLView::getFrameSize()
{
    return _windowSize;
}

void CCEGLView::setFrameSize(const CCSize& size)
{
	if (_window)
		SDL_SetWindowSize(_window, (int)size.width, (int)size.height);
}


bool CCEGLView::createWithSize()
{
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
	// 
	//     SDL_GL_SetAttribute(SDL_GL_MULTISAMPLEBUFFERS, 0);
	//     SDL_GL_SetAttribute(SDL_GL_MULTISAMPLESAMPLES, 0);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

	unsigned flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

//	  flags |= SDL_WINDOW_RESIZABLE;
    flags |= SDL_WINDOW_ALLOW_HIGHDPI;
    
#if BUILD_FOR(IOS) || BUILD_FOR(ANDROID)
	flags |= SDL_WINDOW_FULLSCREEN;
	flags |= SDL_WINDOW_BORDERLESS;
#endif

	_window = SDL_CreateWindow("Cocos2d SDL ", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)_windowSize.width, (int)_windowSize.height, flags);
    
    if (!_window) {
        CCLog("%s", SDL_GetError());
    }
    
    flags = SDL_GetWindowFlags(_window);
    

	SDL_GL_CreateContext(_window);

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
	if (glewInit() != GLEW_OK)
		return false;
    if (GLEW_ARB_vertex_shader && GLEW_ARB_fragment_shader)
    {
        CCLog("Ready for GLSL");
    }
    else
    {
        CCLog("Not totally ready :(");
    }
    
    if (glewIsSupported("GL_VERSION_2_0"))
    {
        CCLog("Ready for OpenGL 2.0");
    }
    else
    {
        CCLog("OpenGL 2.0 not supported");
    }
#endif
    

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 && CC_TARGET_PLATFORM == CC_PLATFORM_MAC
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
#endif 
    SDL_GL_SetSwapInterval(1);
    
    onWindowResized();
    
	return true;
}

bool CCEGLView::isOpenGLReady()
{
	return true;
}

void CCEGLView::end()
{
	SDL_DestroyWindow(_window);
}

void CCEGLView::swapBuffers()
{
	SDL_GL_SwapWindow(_window);
}


void CCEGLView::setIMEKeyboardState(bool /*bOpen*/)
{

}

CCEGLView* CCEGLView::sharedOpenGLView()
{
	if (_instance == NULL)
	{
		CCEGLView* e = new CCEGLView();
		_instance = e;
	}

	return _instance;
}


void CCEGLView::setViewPortInPoints(float x, float y, float w, float h)
{
	glViewport((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}

void CCEGLView::setScissorInPoints(float x, float y, float w, float h)
{
    float sx = _glSize.width / _windowSize.width;
    float sy = _glSize.height / _windowSize.height;
    x *= sx;
    y *= sy;
    w *= sx;
    h *= sy;
	glScissor((GLint)x, (GLint)y, (GLsizei)w, (GLsizei)h);
}

bool CCEGLView::isScissorEnabled()
{
	return (GL_FALSE == glIsEnabled(GL_SCISSOR_TEST)) ? false : true;
}

void CCEGLView::onWindowResized()
{
    int w, h;
    
    SDL_GetWindowSize(_window, &w, &h);
    _windowSize = CCSize((float)w, (float)h);
    
    SDL_GL_GetDrawableSize(_window, &w, &h);
    _glSize = CCSize((float)w, (float)h);
}

NS_CC_END
