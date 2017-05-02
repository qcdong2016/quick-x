#include "CCEGLViewProtocol.h"
#include "engine/CCDirector.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCDictionary.h"

#include "base/MathDefs.h"
NS_CC_BEGIN



CCEGLViewProtocol::CCEGLViewProtocol()
: m_pDelegate(NULL)
{
}

CCEGLViewProtocol::~CCEGLViewProtocol()
{
}

const CCSize& CCEGLViewProtocol::getFrameSize() const
{
    return m_obScreenSize;
}

void CCEGLViewProtocol::setFrameSize(float width, float height)
{
     m_obScreenSize = CCSizeMake(width, height);
}


void CCEGLViewProtocol::setTouchDelegate(EGLTouchDelegate * pDelegate)
{
    m_pDelegate = pDelegate;
}

void CCEGLViewProtocol::setViewPortInPoints(float x , float y , float w , float h)
{
    glViewport((GLint)(x + m_obViewPortRect.origin.x),
               (GLint)(y + m_obViewPortRect.origin.y),
               (GLsizei)(w),
               (GLsizei)(h));
}

void CCEGLViewProtocol::setScissorInPoints(float x , float y , float w , float h)
{
    glScissor((GLint)(x + m_obViewPortRect.origin.x),
              (GLint)(y + m_obViewPortRect.origin.y),
              (GLsizei)(w),
              (GLsizei)(h));
}

bool CCEGLViewProtocol::isScissorEnabled()
{
	return (GL_FALSE == glIsEnabled(GL_SCISSOR_TEST)) ? false : true;
}

CCRect CCEGLViewProtocol::getScissorRect()
{
	GLfloat params[4];
	glGetFloatv(GL_SCISSOR_BOX, params);
	float x = (params[0] - m_obViewPortRect.origin.x);
	float y = (params[1] - m_obViewPortRect.origin.y);
	float w = params[2];
	float h = params[3];
	return CCRectMake(x, y, w, h);
}

const CCRect& CCEGLViewProtocol::getViewPortRect() const
{
    return m_obViewPortRect;
}


NS_CC_END
