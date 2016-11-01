
#include "CCNetwork.h"

NS_CC_BEGIN


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


NS_CC_END
