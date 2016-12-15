#undef CC_EVENT_DEFINE
#undef CC_PARAM
#undef CC_EVENT_END

#define CC_EVENT_DEFINE(name) \
	cocos2d::ID name ::_ID(#name); \
	name::name() : EventData(_count) {} \
	const char* name::getParamName(int param) { \
        switch (param) {

#define CC_EVENT_END() case _count: return nullptr; break; } return nullptr;}
#define CC_PARAM(name) case name: return #name; break;
