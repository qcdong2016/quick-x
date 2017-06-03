/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
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

#include "UITouchGroup.h"

NS_CC_BEGIN

namespace ui {

TouchGroup::TouchGroup()
{
}

TouchGroup::~TouchGroup()
{
    m_pSelectedWidgets->removeAllObjects();
}

bool TouchGroup::init()
{
    if (CCLayer::init())
    {
        m_pSelectedWidgets = CCArray::create();
        m_pRootWidget = Widget::create();
        addChild(m_pRootWidget);
        return true;
    }
    return false;
}

TouchGroup* TouchGroup::create(void)
{
    TouchGroup *pRet = new TouchGroup();
    if (pRet && pRet->init())
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        CC_SAFE_DELETE(pRet);
        return NULL;
    }
}

void TouchGroup::onEnter()
{
    CCLayer::onEnter();
}

void TouchGroup::onExit()
{
    CCLayer::onExit();
}
    
bool TouchGroup::checkEventWidget(CCTouch* touch, CCEvent *pEvent)
{
    return (m_pSelectedWidgets->count() > 0);
}

void TouchGroup::addWidget(Widget* widget)
{
    m_pRootWidget->addChild(widget);
}

void TouchGroup::removeWidget(Widget* widget)
{
    m_pRootWidget->removeChild(widget);
}

Widget* TouchGroup::getRootWidget()
{
    return m_pRootWidget;
}

void TouchGroup::clear()
{
    m_pRootWidget->removeAllChildren();
}
    
}

NS_CC_END