
#ifndef __CC_EXTENSION_CCSTORE_PAYMENT_TRANSACTION_WRAPPER_H_
#define __CC_EXTENSION_CCSTORE_PAYMENT_TRANSACTION_WRAPPER_H_

#include "CCPlatformMacros.h"

NS_CC_BEGIN

class CCStorePaymentTransactionWrapper
{
public:
    static CCStorePaymentTransactionWrapper* createWithTransaction(void* transactionObj);
    ~CCStorePaymentTransactionWrapper(void);
    
    void* getTransactionObj(void)
    {
        return m_transactionObj;
    }
    
private:
    CCStorePaymentTransactionWrapper(void)
    : m_transactionObj(NULL)
    {
    }
    
    void* m_transactionObj;
};

NS_CC_END

#endif // __CC_EXTENSION_CCSTORE_PAYMENT_TRANSACTION_WRAPPER_H_
