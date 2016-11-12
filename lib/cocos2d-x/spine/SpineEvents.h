
CC_EVENT_DEFINE(SpineStart)
	CC_PARAM(trackIndex)
	CC_PARAM(loopCount)
CC_EVENT_END()

CC_EVENT_DEFINE(SpineEnd)
	CC_PARAM(trackIndex)
	CC_PARAM(loopCount)
CC_EVENT_END()


CC_EVENT_DEFINE(SpineComplete)
	CC_PARAM(trackIndex)
	CC_PARAM(loopCount)
	CC_EVENT_END()

CC_EVENT_DEFINE(SpineEvent)
	CC_PARAM(trackIndex)
	CC_PARAM(loopCount)
	CC_PARAM(eventName)
	CC_PARAM(stringValue)
	CC_PARAM(floatValue)
	CC_PARAM(intValue)
CC_EVENT_END()