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

#include "CCScene.h"
#include "cocoa/CCScriptSupport.h"
#include "engine/CCDirector.h"
#include "CCInputEvent.h"

NS_CC_BEGIN

CCScene::CCScene()
{
    m_bIgnoreAnchorPointForPosition = true;
}

CCScene::~CCScene()
{
}

static bool testNode(CCNode* node, const Vec2& pos)
{
	Vec2 nsp = node->convertToNodeSpaceAR(pos);
	Vec2 anchor = node->getAnchorPoint();
	CCSize size = node->getSize();

	CCRect bb = CCRect(-size.width * anchor.x, -size.height * anchor.y, size.width, size.height);
	return bb.containsPoint(nsp);
}

static CCNode* checkNode(int id, CCNode* root, const Vec2& pos)
{
	if (!root->isVisible())
		return nullptr;

	CCArray* children = root->getChildren();
	if (children)
	{
		for (int i = children->count() - 1; i >= 0; i--)
		{
			CCNode* node = checkNode(id, (CCNode*)children->objectAtIndex(i), pos);
			if (node)
				return node;
		}
	}

	if (root->onTouchBegan(id, pos)) {
		return root;
	}

	return nullptr;
}

void CCScene::handleTouchBegin(EventData& data) {
	int id = data[TouchBegin::touchID].GetInt();
	float x = data[TouchBegin::x].GetFloat();
	float y = data[TouchBegin::y].GetFloat();

	Vec2 pos(x, y);

	CCNode* cap = checkNode(id, this, pos);

	if (cap) {
		_touchingNodes[id] = WeakPtr<CCNode>(cap);
	}
}

void CCScene::handleTouchEnd(EventData& data) {
	int id = data[TouchEnd::touchID].GetInt();
	float x = data[TouchEnd::x].GetFloat();
	float y = data[TouchEnd::y].GetFloat();
	
	auto it = _touchingNodes.find(id);
	if (it != _touchingNodes.end()) {
		CCNode* node = it->second;
		if (node) {
			node->onTouchEnded(id, Vec2(x, y));
		}
		_touchingNodes.erase(it);
	}
}

void CCScene::handleTouchMove(EventData& data) {
	int id = data[TouchMove::touchID].GetInt();
	float x = data[TouchMove::x].GetFloat();
	float y = data[TouchMove::y].GetFloat();

	auto it = _touchingNodes.find(id);
	if (it != _touchingNodes.end()) {
		CCNode* node = it->second;
		if (node) {
			node->onTouchMoved(id, Vec2(x, y));
		}
	}
}

bool CCScene::init()
{
	if (CCNode::init()) {
		subscribeToEvent<TouchBegin>(Handler(this, &CCScene::handleTouchBegin));
		subscribeToEvent<TouchEnd>(Handler(this, &CCScene::handleTouchEnd));
		subscribeToEvent<TouchMove>(Handler(this, &CCScene::handleTouchMove));
		return true;
	}
	return false;
}

CCScene *CCScene::create()
{
    CCScene *pRet = new CCScene();
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

CCScene *CCScene::getScene()
{
    return this;
}

NS_CC_END
