
#ifndef __CC_EXTENSION_CCALERTVIEW_DELEGATE_H_
#define __CC_EXTENSION_CCALERTVIEW_DELEGATE_H_

#include "CCPlatformMacros.h"

NS_CC_BEGIN

class CCAlertViewDelegate
{
public:
    virtual void alertViewClickedButtonAtIndex(int buttonIndex) = 0;
};

NS_CC_END

#endif // __CC_EXTENSION_CCALERTVIEW_DELEGATE_H_
