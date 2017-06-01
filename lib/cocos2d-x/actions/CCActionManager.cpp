/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2009      Valentin Milea
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

#include "CCActionManager.h"
#include "nodes/CCNode.h"
#include "CCScheduler.h"
#include "ccMacros.h"
#include "cocoa/data_support/ccCArray.h"
#include "cocoa/data_support/uthash.h"
#include "cocoa/CCSet.h"
#include "engine/CCEngineEvents.h"
#include <algorithm>

NS_CC_BEGIN

CCActionManager::CCActionManager(void)
: _currentTarget(nullptr)
, _currentAction(nullptr)
, _currentTargetRemoved(false)
{
	subscribeToEvent<UpdateEvent>(Handler(this, &CCActionManager::update));
}

CCActionManager::~CCActionManager(void)
{
    CCLOGINFO("cocos2d: deallocing %p", this);

    removeAllActions();
}

void CCActionManager::pauseTarget(CCObject *pTarget)
{
	auto it = _targetToActions.find(pTarget);
	if (it != _targetToActions.end())
		it->second->pause = true;
}

void CCActionManager::resumeTarget(CCObject *pTarget)
{
	auto it = _targetToActions.find(pTarget);
	if (it != _targetToActions.end())
		it->second->pause = false;
}

void CCActionManager::addAction(CCAction *pAction, CCNode *pTarget, bool paused)
{
    CCAssert(pAction != NULL, "");
    CCAssert(pTarget != NULL, "");
	auto it = _targetToActions.find(pTarget);
	ActionInfo* info = nullptr;

	if (it == _targetToActions.end()) 
	{
		info = (_targetToActions[pTarget] = SharedPtr<ActionInfo>(new ActionInfo()));
		info->target = pTarget;
		info->pause = paused;
		info->actions.push_back(SharedPtr<CCAction>(pAction));
	}
	else
	{
		info = it->second;
	}

	pAction->startWithTarget(pTarget);
}

// remove

void CCActionManager::removeAllActions(void)
{
	_targetToActions.clear();
}

void CCActionManager::removeAllActionsFromTarget(CCObject *pTarget)
{
    // explicit null handling
    if (pTarget == NULL)
        return;

	if (_currentTarget && _currentTarget->target == pTarget)
	{
		_currentTargetRemoved = true;
		return;
	}

	auto it = _targetToActions.find(pTarget);
	if (it == _targetToActions.end())
		return;

	it->second->actions.clear();
}

void CCActionManager::removeAction(CCAction *pAction)
{
    // explicit null handling
    if (pAction == NULL)
        return;

	auto it = _targetToActions.find(pAction->getTarget());
	if (it == _targetToActions.end())
		return;
	
	ActionInfo* info = it->second;
		
	if (pAction == info->currentAction)
	{
		info->currentActionRemoved = true;
	}
	else if ((CCObject*)pAction->getTarget() == _currentTarget->target)
	{
		info->currentActionRemoved = true;
	}
	else
	{
		auto i = std::find(info->actions.begin(), info->actions.end(), pAction);
		info->actions.erase(i);
	}
}

void CCActionManager::removeActionByTag(unsigned int tag, CCObject *pTarget)
{
	removeAction(getActionByTag(tag, pTarget));
}

// get
CCAction* CCActionManager::getActionByTag(unsigned int tag, CCObject *pTarget)
{
	auto it = _targetToActions.find(pTarget);
	if (it == _targetToActions.end())
		return nullptr;

	for (const auto& action : it->second->actions)
	{
		if (action->getTag() == (int)tag)
			return action;
	}

    return NULL;
}

unsigned int CCActionManager::numberOfRunningActionsInTarget(CCObject *pTarget)
{
	auto it = _targetToActions.find(pTarget);
	if (it != _targetToActions.end())
		return it->second->actions.size();

    return 0;
}

// main loop
void CCActionManager::update(EventData& data)
{
	float dt = data[UpdateEvent::timeStep].GetFloat();

	for (auto it = _targetToActions.begin(); it != _targetToActions.end();)
	{
		_currentTarget = it->second;
		_currentTargetRemoved = false;

		if (!_currentTarget->pause && !_currentTarget->actions.empty())
		{
			for (auto actionIter = _currentTarget->actions.begin(); actionIter  != _currentTarget->actions.end(); )
			{
				_currentTarget->currentAction = *actionIter;
				if (_currentTarget->currentAction == nullptr)
				{
					actionIter = _currentTarget->actions.erase(actionIter);
					continue;
				}

				_currentTarget->currentActionRemoved = false;
				_currentTarget->currentAction->step(dt);

				if (_currentTargetRemoved)
				{
					break;
				}

				if (_currentTarget->currentActionRemoved)
				{
					actionIter = _currentTarget->actions.erase(actionIter);
				}
				else if (_currentTarget->currentAction->isDone())
				{
					_currentTarget->currentAction->stop();
					_currentTarget->currentAction = nullptr;
					actionIter = _currentTarget->actions.erase(actionIter);
				}
				else
				{
					actionIter++;
				}
				_currentTarget->currentAction = nullptr;
			}
		}

		if (_currentTarget->actions.empty())
		{
			it = _targetToActions.erase(it);
		}
		else
		{
			it++;
		}
	}
	_currentTarget = nullptr;
}

NS_CC_END
