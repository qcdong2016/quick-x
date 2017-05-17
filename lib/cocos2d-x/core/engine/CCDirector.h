/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#ifndef __CCDIRECTOR_H__
#define __CCDIRECTOR_H__

#include "ccTypes.h"
#include "CCGL.h"
#include "platform/CCPlatformMacros.h"
#include "engine/CCObject.h"
#include "cocoa/CCGeometry.h"
#include "cocoa/CCArray.h"
#include "kazmath/mat4.h"
#include "nodes/CCLabelAtlas.h"

#include "engine/CCSubSystem.h"
#include "base/Ptr.h"
#include "CCTimer.h"
#include "nodes/CCScene.h"


NS_CC_BEGIN


class CC_DLL CCApplication : public RefCounted
{
public:
	virtual void applicationWillEnterForeground() = 0;
	virtual void applicationDidEnterBackground() = 0;
	virtual bool applicationDidFinishLaunching() = 0;
};

class CC_DLL Module : public CCObject
{
public:
	virtual ~Module() {};
};

class CC_DLL CCDirectorDelegate
{
public:
	/**
	* Will be called by CCDirector when the projection is updated, and "custom" projection is used
	*/
	virtual void updateProjection(void) = 0;
};

/**
 * @addtogroup base_nodes
 * @{
 */

/** @typedef ccDirectorProjection
 Possible OpenGL projections used by director
 */
typedef enum {
    /// sets a 2D projection (orthogonal projection)
    kCCDirectorProjection2D,
    
    /// sets a 3D projection with a fovy=60, znear=0.5f and zfar=1500.
    kCCDirectorProjection3D,
    
    /// it calls "updateProjection" on the projection delegate.
    kCCDirectorProjectionCustom,
    
    /// Default projection is 3D projection
    kCCDirectorProjectionDefault = kCCDirectorProjection3D,
} ccDirectorProjection;

/* Forward declarations. */
class CCLabelAtlas;
class CCScene;
class CCEGLView;
class CCDirectorDelegate;
class CCNode;
class CCScheduler;
class CCActionManager;
class CCTouchDispatcher;
class Input;

/**
@brief Class that creates and handle the main Window and manages how
and when to execute the Scenes.
 
 The CCDirector is also responsible for:
  - initializing the OpenGL context
  - setting the OpenGL pixel format (default on is RGB565)
  - setting the OpenGL buffer depth (default one is 0-bit)
  - setting the projection (default one is 3D)
  - setting the orientation (default one is Portrait)
 
 Since the CCDirector is a singleton, the standard way to use it is by calling:
  _ CCDirector::sharedDirector()->methodName();
 
 The CCDirector also sets the default OpenGL context:
  - GL_TEXTURE_2D is enabled
  - GL_VERTEX_ARRAY is enabled
  - GL_COLOR_ARRAY is enabled
  - GL_TEXTURE_COORD_ARRAY is enabled
*/
class CC_DLL CCDirector : public CCObject
{
public:
    /**
     *  @js ctor
     */
    CCDirector(CCApplication* app);
    /**
     *  @js NA
     *  @lua NA
     */
    virtual ~CCDirector(void);
    virtual bool init(void);

    // attribute

    /** Get current running Scene. Director can only run one Scene at the time */
    inline CCScene* getScene(void) { return _scene; }
	void setScene(CCScene* sc) { _scene = sc; }

    /** Get the CCEGLView, where everything is rendered
     * @js NA
     */
    inline CCEGLView* getOpenGLView(void) { return m_pobOpenGLView; }
    void setOpenGLView(CCEGLView *pobOpenGLView);

    /** Whether or not the Director is paused */
    inline bool isPaused(void) { return m_bPaused; }

    /** Sets an OpenGL projection
     @since v0.8.2
     @js NA
     */
    inline ccDirectorProjection getProjection(void) { return m_eProjection; }
    void setProjection(ccDirectorProjection kProjection);
     /** reshape projection matrix when canvas has been change"*/
    void reshapeProjection(const CCSize& newWindowSize);
    
    /** Sets the glViewport*/
    void setViewport();

    /** How many frames were called since the director started */
    
    /** CCDirector delegate. It shall implemente the CCDirectorDelegate protocol
     @since v0.99.5
     */
    CCDirectorDelegate* getDelegate() const;
    void setDelegate(CCDirectorDelegate* pDelegate);

    // window size

    /** returns the size of the OpenGL view in points.
    */
    CCSize getWinSize(void);

    /** returns the size of the OpenGL view in pixels.
    */
    CCSize getWinSizeInPixels(void);
    
    /** returns visible size of the OpenGL view in points.
     *  the value is equal to getWinSize if don't invoke
     *  CCEGLView::setDesignResolutionSize()
     */
    CCSize getVisibleSize();
    
    /** returns visible origin of the OpenGL view in points.
     */
    Vec2 getVisibleOrigin();

    /** converts a UIKit coordinate to an OpenGL coordinate
     Useful to convert (multi) touch coordinates to the current layout (portrait or landscape)
     */
    Vec2 convertToGL(const Vec2& obPoint);

    /** converts an OpenGL coordinate to a UIKit coordinate
     Useful to convert node points to window points for calls such as glScissor
     */
    Vec2 convertToUI(const Vec2& obPoint);

    /// XXX: missing description 
    float getZEye(void);

    /** Ends the execution, releases the running scene.
     It doesn't remove the OpenGL view from its parent. You have to do it manually.
     */
    void shutdown(void);

    /** Pauses the running scene.
     The running scene will be _drawed_ but all scheduled timers will be paused
     While paused, the draw rate will be 4 FPS to reduce CPU consumption
     */
    void pause(void);

    /** Resumes the paused scene
     The scheduled timers will be activated again.
     The "delta time" will be 0 (as if the game wasn't paused)
     */
    void resume(void);


    // Memory Helper

    /** Removes cached all cocos2d cached data.
     It will purge the CCTextureCache, CCSpriteFrameCache, CCLabelBMFont cache
     @since v0.99.3
     */
    void purgeCachedData(void);

	/** sets the default values based on the CCConfiguration info */
    void setDefaultValues(void);

    // OpenGL Helper

    /** sets the OpenGL default values */
    void setGLDefaultValues(void);

    /** enables/disables OpenGL alpha blending */
    void setAlphaBlending(bool bOn);

    /** enables/disables OpenGL depth test */
    void setDepthTest(bool bOn);

    virtual void mainLoop(void);

    /** The size in pixels of the surface. It could be different than the screen size.
    High-res devices might have a higher surface size than the screen size.
    Only available when compiled using SDK >= 4.0.
    @since v0.99.4
    */
    void setContentScaleFactor(float scaleFactor);
    float getContentScaleFactor(void);

public:
	
	template<typename T>
	T* addSubSystem() 
	{
		return (T*)addSubSystem(T::getTypeStatic());
	}

	template<typename T>
	T* getSubSystem() 
	{
		return static_cast<T*>(getSubSystem(T::getTypeStatic()));
	}

	SubSystem* addSubSystem(ID type)
	{
		SubSystem* s = ObjectFactoryManager::newObject<SubSystem>(type);
		_subSystems[type] = s;
		return s;
	}

	SubSystem* getSubSystem(ID type)
	{
		auto it = _subSystems.find(type);
		return (it != _subSystems.end()) ? it->second.Get() : nullptr;
	}

    float getDeltaTime() { return _deltaTime; }
	int run();

	bool isRunning();
	void runFrame();
	void timeLimit();
	void setFps(int fps);

	void addModule(SharedPtr<Module> m) { _modules.push_back(m); }
	template<typename T>
	void addModule() { addModule(SharedPtr<Module>(new T));  }
public:
	SharedPtr<Input> input;
    static CCDirector* sharedDirector(void);

protected:
    
    void getFPSImageData(unsigned char** datapointer, unsigned int* length);
	void clear();

protected:

	std::map<ID, SharedPtr<SubSystem> > _subSystems;
	std::vector<SharedPtr<Module> > _modules;

    /* The CCEGLView, where everything is rendered */
    CCEGLView    *m_pobOpenGLView;

    /* landscape mode ? */
    bool m_bLandscape;
    
    /** Whether or not the Director is paused */
    bool m_bPaused;
    
    /* The running scene */
	SharedPtr<CCScene> _scene;
    
    /* projection used */
    ccDirectorProjection m_eProjection;

    /* window size in points */
    CCSize    m_obWinSizeInPoints;
    
    /* content scale factor */
    float    m_fContentScaleFactor;

    /* Projection protocol delegate */
    CCDirectorDelegate *m_pProjectionDelegate;
    
    // CCEGLViewProtocol will recreate stats labels to fit visible rect
    friend class CCEGLViewProtocol;

	int _fps;
	int _pausedFps;
	bool _running;
    float _deltaTime;
	TimerHiRes _frameTimer;

	SharedPtr<CCApplication> _app;
};

// end of base_node group
/// @}

NS_CC_END

#endif // __CCDIRECTOR_H__
