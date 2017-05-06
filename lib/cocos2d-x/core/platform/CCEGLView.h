/****************************************************************************
Copyright (c) 2010 cocos2d-x.org

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

#ifndef __CC_EGL__H__
#define __CC_EGL__H__

#include "CCPlatformDefine.h"
#include "cocoa/CCGeometry.h"

NS_CC_BEGIN


class CC_DLL CCEGLView
{
public:
	CCEGLView();
	virtual ~CCEGLView();

	/* override functions */
	virtual bool isOpenGLReady();
	virtual void end();
	virtual void swapBuffers();
	virtual void setIMEKeyboardState(bool bOpen);
	virtual const CCSize& getFrameSize() const;
	virtual void setFrameSize(const CCSize& size);

	/**
	* Set opengl view port rectangle with points.
	*/
	virtual void setViewPortInPoints(float x, float y, float w, float h);

	/**
	* Set Scissor rectangle with points.
	*/
	virtual void setScissorInPoints(float x, float y, float w, float h);

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
	static CCEGLView* sharedOpenGLView();

private:

	void resize(int width, int height);
	bool createWithSize();
	// real screen size
	CCSize m_obScreenSize;
	// the view port size
	CCRect m_obViewPortRect;
	friend class CCDirector;
};

NS_CC_END

#endif    // end of __CC_EGLVIEW_WIN32_H__
