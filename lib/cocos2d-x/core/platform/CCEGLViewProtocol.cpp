#include "CCEGLViewProtocol.h"
#include "touch_dispatcher/CCTouchDispatcher.h"
#include "touch_dispatcher/CCTouch.h"
#include "engine/CCDirector.h"
#include "cocoa/CCSet.h"
#include "cocoa/CCDictionary.h"

#include "base/MathDefs.h"
NS_CC_BEGIN

static SharedPtr<CCTouch> s_pTouches[CC_MAX_TOUCHES] = {};
static unsigned int s_indexBitsUsed = 0;
//static CCDictionary s_TouchesIntergerDict;

std::map<int, int> s_touchesDict;

static int getUnUsedIndex()
{
    int i;
    int temp = s_indexBitsUsed;

    for (i = 0; i < CC_MAX_TOUCHES; i++) {
        if (! (temp & 0x00000001)) {
            s_indexBitsUsed |= (1 <<  i);
            return i;
        }

        temp >>= 1;
    }

    // all bits are used
    return -1;
}

static void removeUsedIndexBit(int index)
{
    if (index < 0 || index >= CC_MAX_TOUCHES) 
    {
        return;
    }

    unsigned int temp = 1 << index;
    temp = ~temp;
    s_indexBitsUsed &= temp;
}

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

void CCEGLViewProtocol::handleTouchesBegin(int num, int ids[], float xs[], float ys[])
{
    CCSet set;
    for (int i = 0; i < num; ++i)
    {
        int id = ids[i];
        float x = xs[i];
        float y = ys[i];

        //CCInteger* pIndex = (CCInteger*)s_TouchesIntergerDict.objectForKey(id);
		auto iter = s_touchesDict.find(id);
        int nUnusedIndex = 0;

        // it is a new touch
        if (iter == s_touchesDict.end())
        {
            nUnusedIndex = getUnUsedIndex();

            // The touches is more than MAX_TOUCHES ?
            if (nUnusedIndex == -1) {
                CCLOG("The touches is more than MAX_TOUCHES, nUnusedIndex = %d", nUnusedIndex);
                continue;
            }

            CCTouch* pTouch = s_pTouches[nUnusedIndex] = new CCTouch();
			pTouch->setTouchInfo(nUnusedIndex, (x - m_obViewPortRect.origin.x), 
                                     (y - m_obViewPortRect.origin.y));
            
            //CCLOG("x = %f y = %f", pTouch->getLocationInView().x, pTouch->getLocationInView().y);
            
			s_touchesDict[id] = nUnusedIndex;
            set.addObject(pTouch);
        }
    }

    if (set.count() == 0)
    {
        CCLOG("touchesBegan: count = 0");
        return;
    }

    m_pDelegate->touchesBegan(&set, NULL);
}

void CCEGLViewProtocol::handleTouchesMove(int num, int ids[], float xs[], float ys[])
{
    CCSet set;
    for (int i = 0; i < num; ++i)
    {
        int id = ids[i];
        float x = xs[i];
        float y = ys[i];

		auto iter = s_touchesDict.find(id);
		if (iter == s_touchesDict.end()) {
            CCLOG("if the index doesn't exist, it is an error");
            continue;
        }

		int value = iter->second;
        CCLOGINFO("Moving touches with id: %d, x=%f, y=%f", id, x, y);
        CCTouch* pTouch = s_pTouches[value];
        if (pTouch)
        {
			pTouch->setTouchInfo(value, (x - m_obViewPortRect.origin.x),
								(y - m_obViewPortRect.origin.y));
            
            set.addObject(pTouch);
        }
        else
        {
            // It is error, should return.
            CCLOG("Moving touches with id: %d error", id);
            return;
        }
    }

    if (set.count() == 0)
    {
        CCLOG("touchesMoved: count = 0");
        return;
    }

    m_pDelegate->touchesMoved(&set, NULL);
}

void CCEGLViewProtocol::getSetOfTouchesEndOrCancel(CCSet& set, int num, int ids[], float xs[], float ys[])
{
    for (int i = 0; i < num; ++i)
    {
        int id = ids[i];
        float x = xs[i];
        float y = ys[i];

		auto iter = s_touchesDict.find(id);
		if (iter == s_touchesDict.end()) {
            CCLOG("if the index doesn't exist, it is an error");
            continue;
        }
		int value = iter->second;
		/* Add to the set to send to the director */
        CCTouch* pTouch = s_pTouches[value];        
        if (pTouch)
        {
            CCLOGINFO("Ending touches with id: %d, x=%f, y=%f", id, x, y);
			pTouch->setTouchInfo(value, (x - m_obViewPortRect.origin.x),
								(y - m_obViewPortRect.origin.y));

            set.addObject(pTouch);

            s_pTouches[value] = NULL;
            removeUsedIndexBit(value);

			s_touchesDict.erase(id);

        } 
        else
        {
            CCLOG("Ending touches with id: %d error", id);
            return;
        } 

    }

    if (set.count() == 0)
    {
        CCLOG("touchesEnded or touchesCancel: count = 0");
        return;
    }
}

void CCEGLViewProtocol::handleTouchesEnd(int num, int ids[], float xs[], float ys[])
{
    CCSet set;
    getSetOfTouchesEndOrCancel(set, num, ids, xs, ys);
    m_pDelegate->touchesEnded(&set, NULL);
}

void CCEGLViewProtocol::handleTouchesCancel(int num, int ids[], float xs[], float ys[])
{
    CCSet set;
    getSetOfTouchesEndOrCancel(set, num, ids, xs, ys);
    m_pDelegate->touchesCancelled(&set, NULL);
}

const CCRect& CCEGLViewProtocol::getViewPortRect() const
{
    return m_obViewPortRect;
}


NS_CC_END
