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

#include "engine/CCObject.h"
#include "ccMacros.h"
#include "script_support/CCScriptSupport.h"
#include "cocoa/CCAutoreleasePool.h"
#include <vector>
#include <map>

NS_CC_BEGIN


TypeInfo::TypeInfo(const char* typeName, const TypeInfo* superTypeInfo) :
	_type(typeName),
	_typeName(typeName),
	_superTypeInfo(superTypeInfo)
{
}

TypeInfo::~TypeInfo()
{
}

bool TypeInfo::isTypeOf(ID type) const
{
	const TypeInfo* current = this;
	while (current)
	{
		if (current->getType() == type)
			return true;

		current = current->getBaseTypeInfo();
	}

	return false;
}

bool TypeInfo::isTypeOf(const TypeInfo* typeInfo) const
{
	const TypeInfo* current = this;
	while (current)
	{
		if (current == typeInfo)
			return true;

		current = current->getBaseTypeInfo();
	}

	return false;
}


static std::map<EventID, std::set<CCObject*> > eventReceivers;
static std::map<CCObject*, std::map<EventID, std::set<CCObject*> > > specificEventReceivers;

static void removeEventSender(CCObject* sender)
{
	auto i = specificEventReceivers.find(sender);
	if (i != specificEventReceivers.end())
	{
		for (auto j = i->second.begin(); j != i->second.end(); ++j)
		{
			for (auto k = j->second.begin(); k != j->second.end(); ++k)
				(*k)->removeEventSender(sender);
		}
		specificEventReceivers.erase(i);
	}
}

void cocos2d::CCObject::removeEventSender(CCObject* sender)
{
	EventHandler* handler = _eventHandlers.first();
	EventHandler* previous = 0;

	while (handler)
	{
		if (handler->getSender() == sender)
		{
			EventHandler* next = _eventHandlers.next(handler);
			_eventHandlers.erase(handler, previous);
			handler = next;
		}
		else
		{
			previous = handler;
			handler = _eventHandlers.next(handler);
		}
	}
}

static std::set<CCObject*>* getEventReceivers(EventID eventType, CCObject* sender = 0)
{
	if (sender)
	{
		auto i = specificEventReceivers.find(sender);
		if (i != specificEventReceivers.end())
		{
			auto j = i->second.find(eventType);
			return j != i->second.end() ? &j->second : 0;
		}
		
		return 0;
	}
	else
	{
		auto i = eventReceivers.find(eventType);
		return i != eventReceivers.end() ? &i->second : 0;
	}
}

static void addEventReciver(CCObject* reciver, EventID eventType)
{
	std::set<CCObject*>* group = getEventReceivers(eventType);
	if (!group) {
		group = &(eventReceivers[eventType] = std::set<CCObject*>());
	}

	group->insert(reciver);
}

static void removeEventReciver(CCObject* reciver, EventID eventType, CCObject* sender = 0)
{
	std::set<CCObject*>* group = getEventReceivers(eventType, sender);
	if (group)
		group->erase(reciver);
}


#if COCOS2D_DEBUG > 0
int CCObject::s_createdInFrameCount = 0;
int CCObject::s_removedInFrameCount = 0;
int CCObject::s_livingCount = 0;
#endif

CCObject::CCObject(void)
	: m_nLuaID(0)
	, m_uReference(1) // when the object is created, the reference count of it is 1
	, m_uAutoReleaseCount(0)
{
	static unsigned int uObjectCount = 0;

	m_uID = ++uObjectCount;
#if COCOS2D_DEBUG > 0
	++s_createdInFrameCount;
	++s_livingCount;
#endif
}

CCObject::~CCObject(void)
{
#if COCOS2D_DEBUG > 0
	++s_removedInFrameCount;
	--s_livingCount;
#endif

	// if the object is managed, we should remove it
	// from pool manager
	if (m_uAutoReleaseCount > 0)
	{
		CCPoolManager::sharedPoolManager()->removeObject(this);
	}

	// if the object is referenced by Lua engine, remove it
	if (m_nLuaID)
	{
		CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptObjectByCCObject(this);
	}
	else
	{
		CCScriptEngineProtocol* pEngine = CCScriptEngineManager::sharedManager()->getScriptEngine();
		if (pEngine != NULL && pEngine->getScriptType() == kScriptTypeJavascript)
		{
			pEngine->removeScriptObjectByCCObject(this);
		}
	}

	unsubscribeFromAllEvents();
	removeEventSender(this);
}

EventHandler* CCObject::findEventHandler(CCObject* sender, EventHandler** previous)
{
	EventHandler* handler = _eventHandlers.first();
	if (previous)
		*previous = 0;

	while (handler)
	{
		if (handler->getSender() == sender)
			return handler;
		if (previous)
			*previous = handler;

		handler = _eventHandlers.next(handler);
	}

	return 0;
}

EventHandler* CCObject::findEventHandler(CCObject* sender, EventID eventType, EventHandler** previous)
{
	EventHandler* handler = _eventHandlers.first();
	if (previous)
		*previous = 0;

	while (handler)
	{
		if (handler->getSender() == sender && handler->getEventType() == eventType)
			return handler;
		if (previous)
			*previous = handler;

		handler = _eventHandlers.next(handler);
	}

	return 0;
}

void CCObject::subscribeToEvent(EventID eventType, EventHandler* handler)
{
	if (!handler)
		return;

	handler->setSenderAndEventType(nullptr, eventType);

	EventHandler* previous;
	EventHandler* oldHandler = findEventHandler(0, eventType, &previous);

	if (oldHandler)
		_eventHandlers.erase(oldHandler);

	_eventHandlers.insertFront(handler);

	addEventReciver(this, eventType);
}

void CCObject::subscribeToEvent(CCObject* sender, EventID eventType, EventHandler* handler)
{
	// If a null sender was specified, the event can not be subscribed to. Delete the handler in that case
	if (!sender || !handler)
	{
		delete handler;
		return;
	}

	handler->setSenderAndEventType(sender, eventType);

	EventHandler* previous;
	EventHandler* oldHandler = findEventHandler(sender, eventType, &previous);

	_eventHandlers.insertFront(handler);
	addEventReciver(this, eventType);
}

void CCObject::unsubscribeFromAllEvents()
{
	for (;;)
	{
		EventHandler* handler = _eventHandlers.first();
		if (!handler) break;
		removeEventReciver(this, handler->getEventType(), handler->getSender());
		_eventHandlers.erase(handler);
	}
}

void CCObject::unsubscribeFromEvent(EventID eventType)
{
	for (;;)
	{
		EventHandler* previous;
		EventHandler* handler = findEventHandler(0, eventType, &previous);

		if (!handler) break;

		removeEventReciver(this, eventType, handler->getSender());
		_eventHandlers.erase(handler, previous);
	}
}

void CCObject::unsubscribeFromEvent(CCObject* sender, EventID eventType)
{
	if (!sender) return;

	for (;;)
	{
		EventHandler* previous;
		EventHandler* handler = findEventHandler(sender, eventType, &previous);

		if (!handler) break;

		removeEventReciver(this, eventType, handler->getSender());
		_eventHandlers.erase(handler, previous);
	}
}

void CCObject::unsubscribeFromEvents(CCObject* sender)
{
	if (!sender)
		return;

	for (;;)
	{
		EventHandler* previous;
		EventHandler* handler = findEventHandler(sender, &previous);
		if (handler)
		{
			removeEventReciver(this, handler->getEventType(), handler->getSender());
			_eventHandlers.erase(handler, previous);
		}
		else
			break;
	}
}

void CCObject::sendEvent(EventID eventType)
{
	EventDataMap map;
	sendEvent(eventType, map);
}

void CCObject::sendEvent(EventID eventType, EventDataMap& eventData)
{
	WeakPtr<CCObject> self(this);
	std::set<CCObject*> processed;

	std::set<CCObject*>* group = getEventReceivers(eventType, this);

	if (group)
	{
		for (auto iter = group->begin(); iter != group->end();)
		{
			auto current = iter++;
			CCObject* reciver = *current;
			CCObject* next = 0;

			if (iter != group->end())
				next = *iter;

			unsigned oldSize = group->size();
			reciver->onEvent(this, eventType, eventData);

			// If self has been destroyed as a result of event handling, exit
			if (self.Expired())
				return;

			// If group has changed size during iteration (removed/added subscribers) try to recover
			if (group->size() != oldSize)
				iter = group->find(next);
			
			processed.insert(reciver);
		}
	}

	group = getEventReceivers(eventType);
	if (group)
	{
		if (processed.empty())
		{
			for (auto iter = group->begin(); iter != group->end();)
			{
				auto current = iter++;
				CCObject* reciver = *current;
				CCObject* next = 0;

				if (iter != group->end())
					next = *iter;

				unsigned oldSize = group->size();
				reciver->onEvent(this, eventType, eventData);

				if (self.Expired())
					return;

				if (group->size() != oldSize)
					iter = group->find(next);
			}
		}
		else
		{
			for (auto iter = group->begin(); iter != group->end();)
			{
				auto current = iter++;
				CCObject* reciver = *current;
				CCObject* next = 0;

				if (iter != group->end())
					next = *iter;

				if (processed.find(reciver) == processed.end())
				{
					unsigned oldSize = group->size();
					reciver->onEvent(this, eventType, eventData);

					if (self.Expired())
						return;

					if (group->size() != oldSize)
						iter = group->find(next);
				}
			}
		}
	}
}

void CCObject::onEvent(CCObject* sender, EventID eventType, EventDataMap& eventData)
{
	EventHandler* specific = 0;
	EventHandler* nonSpecific = 0;

	EventHandler* handler = _eventHandlers.first();

	while (handler)
	{
		if (handler->getEventType() == eventType)
		{
			if (!handler->getSender())
				nonSpecific = handler;
			else if (handler->getSender() == sender)
			{
				specific = handler;
				break;
			}
		}
		handler = _eventHandlers.next(handler);
	}

	// Specific event handlers have priority, so if found, invoke first
	if (specific)
	{
		specific->invoke(eventData);
		return;
	}

	if (nonSpecific)
	{
		nonSpecific->invoke(eventData);
	}
}


void CCObject::release(void)
{
    CCAssert(m_uReference > 0, "reference count should greater than 0");
    --m_uReference;

    if (m_uReference == 0)
    {
        delete this;
    }
}

void CCObject::retain(void)
{
    CCAssert(m_uReference > 0, "reference count should greater than 0");

    ++m_uReference;
}

CCObject* CCObject::autorelease(void)
{
    CCPoolManager::sharedPoolManager()->addObject(this);
    return this;
}

bool CCObject::isSingleReference(void) const
{
    return m_uReference == 1;
}

unsigned int CCObject::retainCount(void) const
{
    return m_uReference;
}

void cocos2d::CCObject::paste(CCObject* o)
{

}

bool CCObject::isEqual(const CCObject *pObject)
{
    return this == pObject;
}

CCObject* CCObject::copy()
{
	CCObject* o = new CCObject;
	o->autorelease();
	return o;
}


static std::vector<EventID> s_allEvents;

EventID CCObject::findEventID(const char* name)
{
	for (EventID id : s_allEvents)
	{
		if (strcmp(id, name) == 0)
			return id;
	}

	return nullptr;
}

EventID CCObject::regEvent(EventID id)
{
	s_allEvents.push_back(id);
	return id;
}


static std::map<ID, SharedPtr<ObjectFactoryBase> > factorys;

void ObjectFactoryManager::addFactory(ID type, ObjectFactoryBase* factory)
{
	factorys[type] = factory;
}

CCObject* ObjectFactoryManager::createObject(ID type)
{
	auto iter = factorys.find(type);
	if (iter == factorys.end())
		return nullptr;

	return iter->second->newObject();
}

NS_CC_END
