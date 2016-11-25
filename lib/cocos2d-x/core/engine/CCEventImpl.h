#undef CC_EVENT_DEFINE
#undef CC_PARAM
#undef CC_EVENT_END

#define CC_EVENT_DEFINE(name) \
	cocos2d::ID name ::_ID(#name); \
	const char* name ::eventsName[] = {

#define CC_EVENT_END() nullptr};
#define CC_PARAM(name) #name,