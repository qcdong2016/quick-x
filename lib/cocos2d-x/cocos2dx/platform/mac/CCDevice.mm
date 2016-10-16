#include "platform/CCDevice.h"

NS_CC_BEGIN

int CCDevice::getDPI()
{
//TODO: 
    return 160;
}

std::string CCDevice::getWritablePath()
{
    // save to document folder
    NSArray *paths = NSSearchPathForDirectoriesInDomains(NSCachesDirectory, NSUserDomainMask, YES);
    NSString *documentsDirectory = [paths objectAtIndex:0];
    std::string strRet = [documentsDirectory UTF8String];
    strRet.append("/");
    return strRet;
}

NS_CC_END
