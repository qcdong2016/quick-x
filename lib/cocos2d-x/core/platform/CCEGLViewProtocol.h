#ifndef __CCEGLVIEWPROTOCOL_H__
#define __CCEGLVIEWPROTOCOL_H__

#include "ccTypes.h"

NS_CC_BEGIN

#define CC_MAX_TOUCHES  5

class EGLTouchDelegate;
class CCSet;

/**
 * @addtogroup platform
 * @{
 * @js NA
 */

class CC_DLL CCEGLViewProtocol
{
public:
    /**
     * @lua NA
     */
    CCEGLViewProtocol();
    /**
     * @lua NA
     */
    virtual ~CCEGLViewProtocol();

    /** Force destroying EGL view, subclass must implement this method. 
     * @lua NA
     */
    virtual void    end() = 0;

    /** Get whether opengl render system is ready, subclass must implement this method. 
     * @lua NA
     */
    virtual bool    isOpenGLReady() = 0;

    /** Exchanges the front and back buffers, subclass must implement this method. 
     * @lua NA
     */
    virtual void    swapBuffers() = 0;

    /** Open or close IME keyboard , subclass must implement this method. 
     * @lua NA
     */
    virtual void    setIMEKeyboardState(bool bOpen) = 0;

    /**
     * Get the frame size of EGL view.
     * In general, it returns the screen size since the EGL view is a fullscreen view.
     */
    virtual const CCSize& getFrameSize() const;

    /**
     * Set the frame size of EGL view.
     */
    virtual void setFrameSize(float width, float height);

    /** Set touch delegate */
    virtual void setTouchDelegate(EGLTouchDelegate * pDelegate);

    /**
     * Set opengl view port rectangle with points.
     */
    virtual void setViewPortInPoints(float x , float y , float w , float h);

    /**
     * Set Scissor rectangle with points.
     */
    virtual void setScissorInPoints(float x , float y , float w , float h);

    /**
     * Get whether GL_SCISSOR_TEST is enable
     * @lua NA
     */
    virtual bool isScissorEnabled();

    /**
     * Get the current scissor rectangle
     * @lua NA
     */
    virtual CCRect getScissorRect();

    /**
     * Get the opengl view port rectangle.
     */
    const CCRect& getViewPortRect() const;

private:
    void getSetOfTouchesEndOrCancel(CCSet& set, int num, int ids[], float xs[], float ys[]);

protected:
    EGLTouchDelegate* m_pDelegate;

    // real screen size
    CCSize m_obScreenSize;
    // the view port size
    CCRect m_obViewPortRect;
};

// end of platform group
/// @}

NS_CC_END

#endif /* __CCEGLVIEWPROTOCOL_H__ */
