#undef CC_EVENT_DEFINE
#undef CC_PARAM
#undef CC_EVENT_END

#define CC_EVENT_DEFINE(eventName) \
	namespace eventName { \
		cocos2d::EventID Param::Name = #eventName; \
		static cocos2d::EventID eventName##_reg = cocos2d::CCObject::regEvent(Param::Name);

#define CC_PARAM(paramName) \
		cocos2d::EventID Param::paramName = #paramName;

#define CC_EVENT_END() }