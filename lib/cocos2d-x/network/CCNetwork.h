
#ifndef __CC_EXTENSION_CCNETWORK_H_
#define __CC_EXTENSION_CCNETWORK_H_

#include "CCHTTPRequest.h"
#include "CCHTTPRequestDelegate.h"

NS_CC_BEGIN

#define kCCNetworkStatusNotReachable     0
#define kCCNetworkStatusReachableViaWiFi 1
#define kCCNetworkStatusReachableViaWWAN 2

class CCNetwork
{
public:
    /** @brief Checks whether a local wifi connection is available */
    static bool isLocalWiFiAvailable(void);

    /** @brief Checks whether the default route is available */
    static bool isInternetConnectionAvailable(void);

    /** @brief Checks the reachability of a particular host name */
    static bool isHostNameReachable(const char* hostName);

    /** @brief Checks Internet connection reachability status */
    static int getInternetConnectionStatus(void);

    static CCHTTPRequest* createHTTPRequest(CCHTTPRequestDelegate* delegate,
                                            const char* url,
                                            int method = kCCHTTPRequestMethodGET);

    static CCHTTPRequest* createHTTPRequestLua(cocos2d::LUA_FUNCTION listener,
                                               const char* url,
                                               int method = kCCHTTPRequestMethodGET);


private:
    CCNetwork(void) {}
};

NS_CC_END

#endif // __CC_EXTENSION_CCNETWORK_H_
