
#include "CCNetwork.h"

NS_CC_BEGIN


CCHTTPRequest* CCNetwork::createHTTPRequest(const char* url,
                                            int method)
{
    CCHTTPRequest* request = CCHTTPRequest::createWithUrl(url, method);
    request->start();
    return request;
}

NS_CC_END
