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

#include "UIPageView.h"

NS_CC_BEGIN

namespace ui {
    


PageView::PageView():
_curPageIdx(0),
_pages(NULL),
_touchMoveDir(PAGEVIEW_TOUCHLEFT),
_touchStartLocation(0.0f),
_touchMoveStartLocation(0.0f),
_movePagePoint(CCPointZero),
_leftChild(NULL),
_rightChild(NULL),
_isAutoScrolling(false),
_autoScrollDistance(0.0f),
_autoScrollSpeed(0.0f),
_autoScrollDir(0),
_childFocusCancelOffset(5.0f),
_pageViewEventListener(NULL),
_pageViewEventSelector(NULL)
{
}

PageView::~PageView()
{
    _pages->removeAllObjects();
    CC_SAFE_RELEASE(_pages);
    _pageViewEventListener = NULL;
    _pageViewEventSelector = NULL;
}

PageView* PageView::create()
{
    PageView* widget = new PageView();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}
    
void PageView::onEnter()
{
    Layout::onEnter();
    scheduleUpdate();
}

bool PageView::init()
{
    if (Layout::init())
    {
        _pages = CCArray::create();
        CC_SAFE_RETAIN(_pages);
        setClippingEnabled(true);
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void PageView::addWidgetToPage(Widget *widget, int pageIdx, bool forceCreate)
{
    if (!widget)
    {
        return;
    }
    if (pageIdx < 0)
    {
        return;
    }
    int pageCount = _pages->count();
    if (pageIdx < 0 || pageIdx >= pageCount)
    {
        if (forceCreate)
        {
            if (pageIdx > pageCount)
            {
                CCLOG("pageIdx is %d, it will be added as page id [%d]",pageIdx,pageCount);
            }
            Layout* newPage = createPage();
            newPage->addChild(widget);
            addPage(newPage);
        }
    }
    else
    {
        Layout * page = static_cast<Layout*>(_pages->objectAtIndex(pageIdx));
        page->addChild(widget);
    }
}

Layout* PageView::createPage()
{
    Layout* newPage = Layout::create();
    newPage->setSize(_pageSize);
    return newPage;
}

void PageView::addPage(Layout* page)
{
    if (!page)
    {
        return;
    }
    if (_pages->containsObject(page))
    {
        return;
    }
    CCSize pSize = page->getSize();
    if (!pSize.equals(_pageSize))
    {
        CCLOG("page size does not match pageview size, it will be force sized!");
        page->setSize(_pageSize);
    }
    page->setPosition(Vec2(getPositionXByIndex(_pages->count()), 0));
    _pages->addObject(page);
    addChild(page);
    updateBoundaryPages();
}

void PageView::insertPage(Layout* page, int idx)
{
    if (idx < 0)
    {
        return;
    }
    if (!page)
    {
        return;
    }

    if (_pages->containsObject(page))
    {
        return;
    }
    
    int pageCount = _pages->count();
    if (idx >= pageCount)
    {
        addPage(page);
    }
    else
    {
        _pages->insertObject(page, idx);
        page->setPosition(Vec2(getPositionXByIndex(idx), 0));
        addChild(page);
        CCSize pSize = page->getSize();
        if (!pSize.equals(_pageSize))
        {
            CCLOG("page size does not match pageview size, it will be force sized!");
            page->setSize(_pageSize);
        }
        int length = _pages->count();
        for (int i=(idx+1); i<length; i++){
            Widget* behindPage = static_cast<Widget*>(_pages->objectAtIndex(i));
            Vec2 formerPos = behindPage->getPosition();
            behindPage->setPosition(Vec2(formerPos.x + _pageSize.width, 0));
        }
        updateBoundaryPages();
    }
}

void PageView::removePage(Layout* page)
{
    if (!page)
    {
        return;
    }
    removeChild(page);
    updateChildrenPosition();
    updateBoundaryPages();
}

void PageView::removePageAtIndex(int index)
{
    if (index < 0 || index >= (int)(_pages->count()))
    {
        return;
    }
    Layout* page = static_cast<Layout*>(_pages->objectAtIndex(index));
    removePage(page);
}
    
void PageView::removeAllPages()
{
    removeAllChildren();
}

void PageView::updateBoundaryPages()
{
    if (_pages->count() <= 0)
    {
        _leftChild = NULL;
        _rightChild = NULL;
        return;
    }
    _leftChild = static_cast<Widget*>(_pages->objectAtIndex(0));
    _rightChild = static_cast<Widget*>(_pages->objectAtIndex(_pages->count()-1));
}

void PageView::setPageSize(const CCSize& sz)
{
	_pageSize = sz;

	if (!_pages)
	{
		return;
	}
	int startX = getSize().width / 2 - _pageSize.width / 2;
	ccArray* arrayPages = _pages->data;
	int length = arrayPages->num;
	for (int i = 0; i < length; i++)
	{
		Layout* page = static_cast<Layout*>(arrayPages->arr[i]);
		page->setSize(_pageSize);
		page->setPosition(Vec2(startX + (i - _curPageIdx)*_pageSize.width, 0));
	}
}

const CCSize& PageView::getPageSize()
{
	return _pageSize;
}

float PageView::getPositionXByIndex(int idx)
{
    return (getPageSize().width*(idx-_curPageIdx));
}
    
void PageView::addChild(CCNode *child)
{
    Layout::addChild(child);
}

void PageView::addChild(CCNode * child, int zOrder)
{
    Layout::addChild(child, zOrder);
}

void PageView::addChild(CCNode *child, int zOrder, int tag)
{
    Layout::addChild(child, zOrder, tag);
}
    
void PageView::removeChild(CCNode *widget)
{
    removeChild(widget, true);
}

void PageView::removeChild(CCNode *child, bool cleanup)
{
    if (_pages->containsObject(child))
    {
        _pages->removeObject(child);
    }
    Layout::removeChild(child, cleanup);
}

void PageView::onSizeChanged()
{
    Layout::onSizeChanged();
    updateChildrenPosition();
}

void PageView::updateChildrenPosition()
{
    if (!_pages)
    {
        return;
    }
    int pageCount = _pages->count();
    if (pageCount <= 0)
    {
        _curPageIdx = 0;
        return;
    }
    if (_curPageIdx >= pageCount)
    {
        _curPageIdx = pageCount-1;
    }
    float pageWidth = getPageSize().width;
	int startX = getSize().width / 2 - _pageSize.width / 2;
	for (int i = 0; i < pageCount; i++)
    {
        Layout* page = static_cast<Layout*>(_pages->objectAtIndex(i));
        page->setPosition(Vec2(startX + (i-_curPageIdx)*pageWidth, 0));
	}
}

void PageView::removeAllChildren()
{
    removeAllChildrenWithCleanup(true);
}

void PageView::removeAllChildrenWithCleanup(bool cleanup)
{
    _curPageIdx = 0;
    _pages->removeAllObjects();
    Layout::removeAllChildrenWithCleanup(cleanup);
}

void PageView::scrollToPage(int idx)
{
    if (idx < 0 || idx >= (int)(_pages->count()))
    {
        return;
    }
    _curPageIdx = idx;
    Widget* curPage = static_cast<Widget*>(_pages->objectAtIndex(idx));
	int startX = getSize().width / 2 - _pageSize.width / 2;
    _autoScrollDistance = -(curPage->getPosition().x - startX);
    _autoScrollSpeed = fabs(_autoScrollDistance)/0.2f;
    _autoScrollDir = _autoScrollDistance > 0 ? 1 : 0;
    _isAutoScrolling = true;
}

void PageView::update(float dt)
{
    if (_isAutoScrolling)
    {
        switch (_autoScrollDir)
        {
            case 0:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance + step >= 0.0f)
                {
                    step = -_autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance += step;
                }
                scrollPages(-step);
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
                break;
            case 1:
            {
                float step = _autoScrollSpeed*dt;
                if (_autoScrollDistance - step <= 0.0f)
                {
                    step = _autoScrollDistance;
                    _autoScrollDistance = 0.0f;
                    _isAutoScrolling = false;
                }
                else
                {
                    _autoScrollDistance -= step;
                }
                scrollPages(step);
                if (!_isAutoScrolling)
                {
                    pageTurningEvent();
                }
                break;
            }
            default:
                break;
        }
    }
}

bool PageView::onTouchBegan(int id, const Vec2& pos)
{
    bool pass = Layout::onTouchBegan(id, pos);
	if (_hitted)
    {
        handlePressLogic(pos);
    }
    return pass;
}

void PageView::onTouchMoved(int id, const Vec2& pos)
{
	_touchMovePos = pos;
    handleMoveLogic(_touchMovePos);
	Widget* widgetParent = getWidgetParent();
    if (widgetParent)
    {
        widgetParent->checkChildInfo(1,this,_touchMovePos);
    }
    moveEvent();
}

void PageView::onTouchEnded(int id, const Vec2& pos)
{
    Layout::onTouchEnded(id, pos);
    handleReleaseLogic(_touchEndPos);
}

void PageView::movePages(float offset)
{
    ccArray* arrayPages = _pages->data;
    int length = arrayPages->num;
    for (int i=0; i<length; i++)
    {
        Layout* page = static_cast<Layout*>(arrayPages->arr[i]);
        _movePagePoint.x = page->getPosition().x + offset;
        _movePagePoint.y = page->getPosition().y;
        page->setPosition(_movePagePoint);
    }
}

bool PageView::scrollPages(float touchOffset)
{
    if (_pages->count() <= 0)
    {
        return false;
    }
    
    if (!_leftChild || !_rightChild)
    {
        return false;
    }
    
    float realOffset = touchOffset;
	int selfWidth = getSize().width;
	int startX = getSize().width / 2 - _pageSize.width / 2;

	switch (_touchMoveDir)
	{
	case PAGEVIEW_TOUCHLEFT: // left
		{
			int right = _rightChild->getPositionX();
			right += _pageSize.width / 2;
			
			if (right + touchOffset <= selfWidth / 2)
			{
				return false;
			}
			break;
		}
        case PAGEVIEW_TOUCHRIGHT: // right 
		{
			int left = _leftChild->getPositionX();
			left += _pageSize.width / 2;
			if (left + touchOffset >= selfWidth / 2)
			{
				return false;
			}
			break;
		}
		default:
            break;
    }
    
    movePages(realOffset);
    return true;
}

void PageView::handlePressLogic(const Vec2 &touchPoint)
{
    Vec2 nsp = convertToNodeSpace(touchPoint);
    _touchMoveStartLocation = nsp.x;
    _touchStartLocation = nsp.x;
}

void PageView::handleMoveLogic(const Vec2 &touchPoint)
{
    Vec2 nsp = convertToNodeSpace(touchPoint);
    float offset = 0.0;
    float moveX = nsp.x;
    offset = moveX - _touchMoveStartLocation;
    _touchMoveStartLocation = moveX;
    if (offset < 0)
    {
        _touchMoveDir = PAGEVIEW_TOUCHLEFT;
    }
    else if (offset > 0)
    {
        _touchMoveDir = PAGEVIEW_TOUCHRIGHT;
    }
    scrollPages(offset);
}

void PageView::handleReleaseLogic(const Vec2 &touchPoint)
{
    if (_pages->count() <= 0)
    {
        return;
    }
    Widget* curPage = static_cast<Widget*>(_pages->objectAtIndex(_curPageIdx));
    if (curPage)
    {
        Vec2 curPagePos = curPage->getPosition();
        int pageCount = _pages->count();
		int startX = getSize().width / 2 - _pageSize.width / 2;
		float curPageLocation = curPagePos.x - startX;
        float pageWidth = getPageSize().width;
        float boundary = pageWidth/2.0f;
        if (curPageLocation <= -boundary)
        {
            if (_curPageIdx >= pageCount-1)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(_curPageIdx+1);
            }
        }
        else if (curPageLocation >= boundary)
        {
            if (_curPageIdx <= 0)
            {
                scrollPages(-curPageLocation);
            }
            else
            {
                scrollToPage(_curPageIdx-1);
            }
        }
        else
        {
            scrollToPage(_curPageIdx);
        }
    }
}

void PageView::checkChildInfo(int handleState,Widget* sender, const Vec2 &touchPoint)
{
    interceptTouchEvent(handleState, sender, touchPoint);
}

void PageView::interceptTouchEvent(int handleState, Widget *sender, const Vec2 &touchPoint)
{
    switch (handleState)
    {
        case 0:
            handlePressLogic(touchPoint);
            break;
        case 1:
        {
            float offset = 0;
            offset = fabs(sender->getTouchStartPos().x - touchPoint.x);
            if (offset > _childFocusCancelOffset)
            {
                sender->setFocused(false);
                handleMoveLogic(touchPoint);
            }
        }
            break;
        case 2:
            handleReleaseLogic(touchPoint);
            break;
            handleReleaseLogic(touchPoint);
        case 3:
            break;
    }
}

void PageView::pageTurningEvent()
{
    if (_pageViewEventListener && _pageViewEventSelector)
    {
        (_pageViewEventListener->*_pageViewEventSelector)(this, PAGEVIEW_EVENT_TURNING);
    }
}

void PageView::addEventListenerPageView(CCObject *target, SEL_PageViewEvent selector)
{
    _pageViewEventListener = target;
    _pageViewEventSelector = selector;
}

int PageView::getCurPageIndex() const
{
    return _curPageIdx;
}

CCArray* PageView::getPages()
{
    return _pages;
}
    
Layout* PageView::getPage(int index)
{
    if (index < 0 || index >= (int)(_pages->count()))
    {
        return NULL;
    }
    return static_cast<Layout*>(_pages->objectAtIndex(index));
}

std::string PageView::getDescription() const
{
    return "PageView";
}

Widget* PageView::createCloneInstance()
{
    return PageView::create();
}

void PageView::copyClonedWidgetChildren(Widget* model)
{
    ccArray* arrayModelPages = static_cast<PageView*>(model)->getPages()->data;
    int length = arrayModelPages->num;
    for (int i=0; i<length; i++)
    {
        Layout* page = static_cast<Layout*>(arrayModelPages->arr[i]);
        addPage(dynamic_cast<Layout*>(page->clone()));
    }
}

void PageView::copySpecialProperties(Widget *widget)
{
    PageView* pageView = dynamic_cast<PageView*>(widget);
    if (pageView)
    {
        Layout::copySpecialProperties(widget);
    }
}

}

NS_CC_END