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

#ifndef __CCOBJECT_H__
#define __CCOBJECT_H__

#include "CCPlatformMacros.h"
#include "base/RefCounted.h"
#include "base/Variant.h"
#include "base/LinkedList.h"
#include "base/ID.h"
#include <string>
#include <map>

NS_CC_BEGIN

class CCObject;
class EventHandler;

typedef const char* EventID;
typedef std::map<EventID, Variant> EventDataMap;

class CC_DLL TypeInfo
{
public:
	TypeInfo(const char* typeName, const TypeInfo* superTypeInfo);
	virtual ~TypeInfo();

	/// Check current type is type of specified type.
	bool isTypeOf(ID type) const;
	/// Check current type is type of specified type.
	bool isTypeOf(const TypeInfo* typeInfo) const;
	/// Check current type is type of specified class type.
	template<typename T> bool isTypeOf() const { return isTypeOf(T::getTypeInfoStatic()); }

	/// Return type.
	ID getType() const { return _type; }
	/// Return type name.
	const std::string& getTypeName() const { return _typeName; }
	/// Return base type info.
	const TypeInfo* getBaseTypeInfo() const { return _superTypeInfo; }

private:
	/// Type.
	ID _type;
	/// Type name.
	std::string _typeName;
	/// Base class type info.
	const TypeInfo* _superTypeInfo;
};

class ObjectFactoryBase : public RefCounted
{
public:
	virtual CCObject* newObject() = 0;
};

template<typename T>
class ObjectFactory : public ObjectFactoryBase
{
public:
	virtual CCObject* newObject()
	{
		return new T();
	}
};

class ObjectFactoryManager
{
public:
	template<typename T>
	static void addFactory() {
		addFactory(T::getTypeStatic(), new ObjectFactory<T>);
	}
	static void addFactory(ID type, ObjectFactoryBase* factory);
	static CCObject* createObject(ID type);
		template<typename T>
	static T* newObject(ID type) {
		return static_cast<T*>(createObject(type));
	}
};

#define CCOBJECT(typeName, superTypeName)                       \
	public:                                                    \
		typedef typeName SelfType;                                \
		typedef superTypeName Super;                                 \
		static const cocos2d::TypeInfo* getTypeInfoStatic() { static cocos2d::TypeInfo typeInfoStatic(#typeName, superTypeName::getTypeInfoStatic()); return &typeInfoStatic; } \
		static const std::string& getTypeNameStatic() { return getTypeInfoStatic()->getTypeName(); } \
		static cocos2d::ID getTypeStatic() { return getTypeInfoStatic()->getType(); } \
		virtual const cocos2d::TypeInfo* getTypeInfo() { return getTypeInfoStatic(); } \
		virtual cocos2d::ID getType() { return getTypeInfoStatic()->getType(); } \
		virtual const std::string& getTypeName() { return getTypeInfoStatic()->getTypeName(); } \
		virtual CCObject* copy()                                   \
		{                                                          \
			CCObject* o = new SelfType();                          \
			o->autorelease();                                      \
			paste(o);                                              \
			return o;                                              \
		}


#define O dynamic_cast<SelfType*>(o)

/**
 * @js NA
 */
class CC_DLL CCObject : public RefCounted
{
    
public:
    // object id, CCScriptSupport need public m_uID
    unsigned int        m_uID;
    // Lua reference id
    int                 m_nLuaID;

protected:
    // count of references
    unsigned int        m_uReference;
    // count of autorelease
    unsigned int        m_uAutoReleaseCount;

public:
    CCObject(void);
    /**
     *  @lua NA
     */
    virtual ~CCObject(void);

	void release(void);
    void retain(void);
    CCObject* autorelease(void);
    bool isSingleReference(void) const;
	unsigned int retainCount(void) const;

	/// paste this proprty to o;
	virtual void paste(CCObject* o);
	virtual CCObject* copy();
	static const TypeInfo* getTypeInfoStatic() { static TypeInfo typeInfoStatic("CCObject", nullptr); return &typeInfoStatic; }
	virtual const TypeInfo* getTypeInfo() { return getTypeInfoStatic(); }
	virtual ID getType() { return getTypeInfo()->getType(); }
	virtual const std::string& getTypeName() { return getTypeInfo()->getTypeName(); }

	virtual bool isEqual(const CCObject* pObject);
    virtual void update(float dt) {CC_UNUSED_PARAM(dt);};

	/// Subscribe to an event that can be sent by any sender.
	void subscribeToEvent(EventID eventType, EventHandler* handler);
	void subscribeToEvent(CCObject* sender, EventID eventType, EventHandler* handler);
	/// Unsubscribe from an event.
	void unsubscribeFromEvent(EventID eventType);
	void unsubscribeFromEvent(CCObject* sender, EventID eventType);
	void unsubscribeFromEvents(CCObject* sender);
	/// Unsubscribe from all events.
	void unsubscribeFromAllEvents();
	/// Send event to all subscribers.
	void sendEvent(EventID eventType);
	void sendEvent(EventID eventType, EventDataMap& eventData);
	template<typename T> void sendEvent() { sendEvent(T::Name); }
	template<typename T> void sendEvent(EventDataMap& eventData) { sendEvent(T::Name, eventData); }

	void onEvent(CCObject* sender, EventID eventType, EventDataMap& eventData);

	EventHandler* findEventHandler(CCObject* sender, EventID eventType, EventHandler** previous);
	EventHandler* findEventHandler(CCObject* sender, EventHandler** previous);

	void removeEventSender(CCObject* sender);

	static EventID findEventID(const char* name);
	static EventID regEvent(EventID id);
private:
	LinkedList<EventHandler> _eventHandlers;

    friend class CCAutoreleasePool;

#if COCOS2D_DEBUG > 0
public:
    // count of created object before autorelease
    static int s_createdInFrameCount;
    // count of removed object after autorelease
    static int s_removedInFrameCount;
    // count of living objects
    static int s_livingCount;

#endif
};

/// Internal helper class for invoking event handler functions.
class CC_DLL EventHandler : public LinkedListNode
{
public:
	/// Construct with specified receiver and userdata.
	EventHandler()
	{
	}

	/// Destruct.
	virtual ~EventHandler() { }

	void setSenderAndEventType(CCObject* sender, EventID eventType)
	{
		_sender = sender;
		_eventType = eventType;
	}

	/// Invoke event handler function.
	virtual void invoke(EventDataMap& eventData) = 0;
	virtual void invoke()
	{
		EventDataMap m;
		invoke(m);
	}
	/// Return a unique copy of the event handler.
	virtual EventHandler* clone() const = 0;

	const EventID& getEventType() const { return _eventType; }
	CCObject* getSender() const { return _sender; }

protected:
	/// Event type.
	EventID _eventType;
	CCObject* _sender;
};

/// Template implementation of the event handler invoke helper (stores a function pointer of specific class.)
template <typename ReceiverType>
class EventHandlerImpl : public EventHandler
{
public:
	typedef void (ReceiverType::*HandlerFunctionPtr)(EventDataMap&);

	/// Construct with receiver and function pointers and userdata.
	EventHandlerImpl(ReceiverType* receiver, HandlerFunctionPtr function) 
		: _receiver(receiver)
		, _function(function)
	{
		assert(_function);
	}

	/// Invoke event handler function.
	virtual void invoke(EventDataMap& eventData)
	{
		(_receiver->*_function)(eventData);
	}

	/// Return a unique copy of the event handler.
	virtual EventHandler* clone() const
	{
		return new EventHandlerImpl(_receiver, _function);
	}

private:
	/// Event receiver.
	ReceiverType* _receiver;
	/// Class-specific pointer to handler function.
	HandlerFunctionPtr _function;
};

template <typename ReceiverType>
class EventHandlerImplNoArg : public EventHandler
{
public:
	typedef void (ReceiverType::*HandlerFunctionPtr)();

	/// Construct with receiver and function pointers and userdata.
	EventHandlerImplNoArg(ReceiverType* receiver, HandlerFunctionPtr function)
		: _receiver(receiver)
		, _function(function)
	{
		assert(_function);
	}

	/// Invoke event handler function.
	virtual void invoke(EventDataMap& eventData)
	{
		(_receiver->*_function)();
	}

	/// Return a unique copy of the event handler.
	virtual EventHandler* clone() const
	{
		return new EventHandlerImplNoArg(_receiver, _function);
	}

private:
	/// Event receiver.
	ReceiverType* _receiver;
	/// Class-specific pointer to handler function.
	HandlerFunctionPtr _function;
};

template <typename ReceiverType>
static inline EventHandler* Handler(ReceiverType* receiver, void (ReceiverType::*function)(EventDataMap&))
{
	return new EventHandlerImpl<ReceiverType>(receiver, function);
}
template <typename ReceiverType>
static inline EventHandler* Handler(ReceiverType* receiver, void (ReceiverType::*function)())
{
	return new EventHandlerImplNoArg<ReceiverType>(receiver, function);
}

#define CC_EVENT_DEFINE(eventName) \
	namespace eventName { \
		class Param { public: \
			static cocos2d::EventID Name;

#define CC_PARAM(paramName) \
		static cocos2d::EventID paramName;

#define CC_EVENT_END() }; }

//
class CCObject;
class CCNode;
class CCEvent;

typedef void (CCObject::*SEL_SCHEDULE)(float);
typedef void (CCObject::*SEL_CallFunc)();
typedef void (CCObject::*SEL_CallFuncN)(CCNode*);
typedef void (CCObject::*SEL_CallFuncND)(CCNode*, void*);
typedef void (CCObject::*SEL_CallFuncO)(CCObject*);
typedef void (CCObject::*SEL_MenuHandler)(CCObject*);
typedef void (CCObject::*SEL_EventHandler)(CCEvent*);
typedef int (CCObject::*SEL_Compare)(CCObject*);

#define schedule_selector(_SELECTOR) (SEL_SCHEDULE)(&_SELECTOR)
#define callfunc_selector(_SELECTOR) (SEL_CallFunc)(&_SELECTOR)
#define callfuncN_selector(_SELECTOR) (SEL_CallFuncN)(&_SELECTOR)
#define callfuncND_selector(_SELECTOR) (SEL_CallFuncND)(&_SELECTOR)
#define callfuncO_selector(_SELECTOR) (SEL_CallFuncO)(&_SELECTOR)
#define menu_selector(_SELECTOR) (SEL_MenuHandler)(&_SELECTOR)
#define event_selector(_SELECTOR) (SEL_EventHandler)(&_SELECTOR)
#define compare_selector(_SELECTOR) (SEL_Compare)(&_SELECTOR)

// end of base_nodes group
/// @}

NS_CC_END

#endif // __CCOBJECT_H__
