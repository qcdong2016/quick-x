
#include "CCNetwork.h"

NS_CC_BEGIN

#if (CC_CURL_ENABLED > 0 || CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)

CCHTTPRequest* CCNetwork::createHTTPRequest(CCHTTPRequestDelegate* delegate,
                                            const char* url,
                                            int method)
{
    CCHTTPRequest* request = CCHTTPRequest::createWithUrl(delegate, url, method);
    request->start();
    return request;
}

CCHTTPRequest* CCNetwork::createHTTPRequestLua(LUA_FUNCTION listener,
                                               const char* url,
                                               int method)
{
    CCHTTPRequest* request = CCHTTPRequest::createWithUrlLua(listener, url, method);
    request->start();
    return request;
}

#endif // CC_CURL_ENABLED

NS_CC_END
