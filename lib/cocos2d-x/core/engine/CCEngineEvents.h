
CC_EVENT_DEFINE(EmptyEvent)
CC_EVENT_END()

CC_EVENT_DEFINE(KeypadEvent)
CC_PARAM(key)
CC_EVENT_END()

CC_EVENT_DEFINE(UpdateEvent)
CC_PARAM(timeStep)
CC_EVENT_END()

CC_EVENT_DEFINE(AfterDraw)
CC_EVENT_END();

CC_EVENT_DEFINE(EnterBackground)
CC_EVENT_END()

CC_EVENT_DEFINE(EnterForeground)
CC_EVENT_END()

CC_EVENT_DEFINE(ContextLost)
CC_EVENT_END()

CC_EVENT_DEFINE(ContextReset)
CC_EVENT_END()

CC_EVENT_DEFINE(AlertEvent)
	CC_PARAM(buttonIndex)
CC_EVENT_END()

CC_EVENT_DEFINE(AccelerationEvent)//CCAccelerometer
CC_PARAM(x)//double
CC_PARAM(y)//double
CC_PARAM(z)//double
CC_PARAM(timeStamp)//double
CC_EVENT_END()