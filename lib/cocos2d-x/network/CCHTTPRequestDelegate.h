
#ifndef __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_
#define __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_


NS_CC_BEGIN

class CCHTTPRequest;

class CCHTTPRequestDelegate
{
public:
    virtual void requestFinished(CCHTTPRequest* request) {}
    virtual void requestFailed(CCHTTPRequest* request) {}
};

NS_CC_END

#endif // __CC_EXTENSION_CCHTTP_REQUEST_DELEGATE_H_