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
#include "cocos/RefCounted.h"
#include "cocos/Variant.h"
#include "cocos/LinkedList.h"
#include <string>

NS_CC_BEGIN

class CCObject;
class EventHandler;

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


#define CCOBJECT(typeName, superTypeName)                       \
	public:                                                    \
		typedef typeName SelfType;                                \
		typedef superTypeName Super;                                 \
		static const cocos2d::TypeInfo* getTypeInfoStatic() { static cocos2d::TypeInfo typeInfoStatic(#typeName, superTypeName::getTypeInfo()); return &typeInfoStatic; } \
		static const std::string& getTypeName() { return getTypeInfoStatic()->getTypeName(); } \
		static const cocos2d::TypeInfo* getTypeInfo() { return getTypeInfoStatic(); } \
		static cocos2d::ID getType() { return getTypeInfoStatic()->getType(); } \
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
	static const TypeInfo* getTypeInfo() { return 0; }

	virtual bool isEqual(const CCObject* pObject);
    virtual void update(float dt) {CC_UNUSED_PARAM(dt);};

	/// Subscribe to an event that can be sent by any sender.
	void subscribeToEvent(ID eventType, EventHandler* handler);
	/// Subscribe to a specific sender's event.
	void subscribeToEvent(CCObject* sender, ID eventType, EventHandler* handler);

	/// Unsubscribe from an event.
	void unsubscribeFromEvent(ID eventType);
	/// Unsubscribe from a specific sender's event.
	void unsubscribeFromEvent(CCObject* sender, ID eventType);
	/// Unsubscribe from a specific sender's events.
	void unsubscribeFromEvents(CCObject* sender);
	/// Unsubscribe from all events.
	void unsubscribeFromAllEvents();
	/// Send event to all subscribers.
	void sendEvent(ID eventType);
	/// Send event with parameters to all subscribers.
	void sendEvent(ID eventType, VariantMap& eventData);

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
	EventHandler(CCObject* receiver, void* userData = 0) :
		_receiver(receiver),
		_userData(userData)
	{
		assert(_receiver);
	}

	/// Destruct.
	virtual ~EventHandler() { }

	/// Invoke event handler function.
	virtual void invoke(VariantMap& eventData) = 0;
	/// Return a unique copy of the event handler.
	virtual EventHandler* clone() const = 0;

	/// Return event receiver.
	CCObject* getReceiver() const { return _receiver; }

	/// Return userdata.
	void* getUserData() const { return _userData; }

protected:
	/// Event type.
	ID _eventType;
	/// Event receiver.
	CCObject* _receiver;
	/// Userdata.
	void* _userData;
};

/// Template implementation of the event handler invoke helper (stores a function pointer of specific class.)
template <typename ReceiverType, typename EventType>
class EventHandlerImpl : public EventHandler
{
public:
	typedef void (ReceiverType::*HandlerFunctionPtr)(CCObject* sender, const EventType&);

	/// Construct with receiver and function pointers and userdata.
	EventHandlerImpl(ReceiverType* receiver, HandlerFunctionPtr function, void* userData = 0) :
		EventHandler(receiver, userData),
		_function(function)
	{
		assert(_function);
	}

	/// Invoke event handler function.
	virtual void invoke(VariantMap& evt)
	{
		ReceiverType* receiver = static_cast<ReceiverType*>(_receiver);
		(receiver->*_function)(_eventType, evt);
	}

	/// Return a unique copy of the event handler.
	virtual EventHandler* clone() const
	{
		return new EventHandlerImpl(static_cast<ReceiverType*>(_receiver), _function, _userData);
	}

private:
	/// Class-specific pointer to handler function.
	HandlerFunctionPtr _function;
};

template <typename EventType, typename ReceiverType>
static inline EventHandler* Handler(ReceiverType* receiver, void (ReceiverType::*function)(CCObject* sender, const EventType&), void* userData = 0)
{
	return new cocos2d::EventHandlerImpl<ReceiverType, EventType>(receiver, function);
}

/// Register event names.
struct CC_DLL EventNameRegistrar
{
	/// Register an event name for hash reverse mapping.
	static ID registerEventName(const char* eventName);
	/// Return Event name or empty string if not found.
	static const std::string& getEventName(ID eventID);
	/// Return Event name map.
	static std::map<ID, std::string>& getEventNameMap();
};

/// Describe an event's hash ID and begin a namespace in which to define its parameters.
#define CC_EVENT(eventName) static const cocos2d::ID eventID(cocos2d::EventNameRegistrar::registerEventName(#eventName)); namespace eventName
/// Describe an event's parameter hash ID. Should be used inside an event namespace.
#define CC_PARAM(paramID, paramName) static const cocos2d::ID paramID(#paramName)


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
