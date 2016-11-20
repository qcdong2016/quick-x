/****************************************************************************
 Copyright (c) 2012 - 2013 cocos2d-x.org
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "CCDictionary.h"
#include "CCString.h"
#include "CCInteger.h"
#include <stack>
#include "CCSAXParser.h"
#include "IO/FileSystem.h"

using namespace std;

NS_CC_BEGIN


typedef enum
{
	SAX_NONE = 0,
	SAX_KEY,
	SAX_DICT,
	SAX_INT,
	SAX_REAL,
	SAX_STRING,
	SAX_ARRAY
}CCSAXState;

typedef enum
{
	SAX_RESULT_NONE = 0,
	SAX_RESULT_DICT,
	SAX_RESULT_ARRAY
}CCSAXResult;

class CCDictMaker : public CCSAXDelegator
{
public:
	CCSAXResult m_eResultType;
	CCArray* m_pRootArray;
	CCDictionary *m_pRootDict;
	CCDictionary *m_pCurDict;
	std::stack<CCDictionary*> m_tDictStack;
	std::string m_sCurKey;   ///< parsed key
	std::string m_sCurValue; // parsed value
	CCSAXState m_tState;
	CCArray* m_pArray;

	std::stack<CCArray*> m_tArrayStack;
	std::stack<CCSAXState>  m_tStateStack;

public:
	CCDictMaker()
		: m_eResultType(SAX_RESULT_NONE),
		m_pRootArray(NULL),
		m_pRootDict(NULL),
		m_pCurDict(NULL),
		m_tState(SAX_NONE),
		m_pArray(NULL)
	{
	}

	~CCDictMaker()
	{
	}

	CCDictionary* dictionaryWithContentsOfData(const char* data, unsigned long size)
	{
		m_eResultType = SAX_RESULT_DICT;
		CCSAXParser parser;

		if (false == parser.init("UTF-8"))
		{
			return NULL;
		}
		parser.setDelegator(this);

		parser.parse(data, size);
		return m_pRootDict;
	}

	CCDictionary* dictionaryWithContentsOfFile(const char *pFileName)
	{
		m_eResultType = SAX_RESULT_DICT;
		CCSAXParser parser;

		if (false == parser.init("UTF-8"))
		{
			return NULL;
		}
		parser.setDelegator(this);

		parser.parse(pFileName);
		return m_pRootDict;
	}

	CCArray* arrayWithContentsOfFile(const char* pFileName)
	{
		m_eResultType = SAX_RESULT_ARRAY;
		CCSAXParser parser;

		if (false == parser.init("UTF-8"))
		{
			return NULL;
		}
		parser.setDelegator(this);

		parser.parse(pFileName);
		return m_pArray;
	}

	void startElement(void *ctx, const char *name, const char **atts)
	{
		CC_UNUSED_PARAM(ctx);
		CC_UNUSED_PARAM(atts);
		std::string sName((char*)name);
		if (sName == "dict")
		{
			m_pCurDict = new CCDictionary();
			if (m_eResultType == SAX_RESULT_DICT && m_pRootDict == NULL)
			{
				// Because it will call m_pCurDict->release() later, so retain here.
				m_pRootDict = m_pCurDict;
				m_pRootDict->retain();
			}
			m_tState = SAX_DICT;

			CCSAXState preState = SAX_NONE;
			if (!m_tStateStack.empty())
			{
				preState = m_tStateStack.top();
			}

			if (SAX_ARRAY == preState)
			{
				// add the dictionary into the array
				m_pArray->addObject(m_pCurDict);
			}
			else if (SAX_DICT == preState)
			{
				// add the dictionary into the pre dictionary
				CCAssert(!m_tDictStack.empty(), "The state is wrong!");
				CCDictionary* pPreDict = m_tDictStack.top();
				pPreDict->setObject(m_pCurDict, m_sCurKey.c_str());
			}

			m_pCurDict->release();

			// record the dict state
			m_tStateStack.push(m_tState);
			m_tDictStack.push(m_pCurDict);
		}
		else if (sName == "key")
		{
			m_tState = SAX_KEY;
		}
		else if (sName == "integer")
		{
			m_tState = SAX_INT;
		}
		else if (sName == "real")
		{
			m_tState = SAX_REAL;
		}
		else if (sName == "string")
		{
			m_tState = SAX_STRING;
		}
		else if (sName == "array")
		{
			m_tState = SAX_ARRAY;
			m_pArray = new CCArray();
			if (m_eResultType == SAX_RESULT_ARRAY && m_pRootArray == NULL)
			{
				m_pRootArray = m_pArray;
				m_pRootArray->retain();
			}
			CCSAXState preState = SAX_NONE;
			if (!m_tStateStack.empty())
			{
				preState = m_tStateStack.top();
			}

			if (preState == SAX_DICT)
			{
				m_pCurDict->setObject(m_pArray, m_sCurKey.c_str());
			}
			else if (preState == SAX_ARRAY)
			{
				CCAssert(!m_tArrayStack.empty(), "The state is wrong!");
				CCArray* pPreArray = m_tArrayStack.top();
				pPreArray->addObject(m_pArray);
			}
			m_pArray->release();
			// record the array state
			m_tStateStack.push(m_tState);
			m_tArrayStack.push(m_pArray);
		}
		else
		{
			m_tState = SAX_NONE;
		}
	}

	void endElement(void *ctx, const char *name)
	{
		CC_UNUSED_PARAM(ctx);
		CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
		std::string sName((char*)name);
		if (sName == "dict")
		{
			m_tStateStack.pop();
			m_tDictStack.pop();
			if (!m_tDictStack.empty())
			{
				m_pCurDict = m_tDictStack.top();
			}
		}
		else if (sName == "array")
		{
			m_tStateStack.pop();
			m_tArrayStack.pop();
			if (!m_tArrayStack.empty())
			{
				m_pArray = m_tArrayStack.top();
			}
		}
		else if (sName == "true")
		{
			CCString *str = new CCString("1");
			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(str);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(str, m_sCurKey.c_str());
			}
			str->release();
		}
		else if (sName == "false")
		{
			CCString *str = new CCString("0");
			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(str);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(str, m_sCurKey.c_str());
			}
			str->release();
		}
		else if (sName == "string" || sName == "integer" || sName == "real")
		{
			CCString* pStrValue = new CCString(m_sCurValue);

			if (SAX_ARRAY == curState)
			{
				m_pArray->addObject(pStrValue);
			}
			else if (SAX_DICT == curState)
			{
				m_pCurDict->setObject(pStrValue, m_sCurKey.c_str());
			}

			pStrValue->release();
			m_sCurValue.clear();
		}

		m_tState = SAX_NONE;
	}

	void textHandler(void *ctx, const char *ch, int len)
	{
		CC_UNUSED_PARAM(ctx);
		if (m_tState == SAX_NONE)
		{
			return;
		}

		CCSAXState curState = m_tStateStack.empty() ? SAX_DICT : m_tStateStack.top();
		CCString *pText = new CCString(std::string((char*)ch, 0, len));

		switch (m_tState)
		{
		case SAX_KEY:
			m_sCurKey = pText->getCString();
			break;
		case SAX_INT:
		case SAX_REAL:
		case SAX_STRING:
		{
			if (curState == SAX_DICT)
			{
				CCAssert(!m_sCurKey.empty(), "key not found : <integer/real>");
			}

			m_sCurValue.append(pText->getCString());
		}
		break;
		default:
			break;
		}
		pText->release();
	}
};

// -----------------------------------------------------------------------
// CCDictElement

CCDictElement::CCDictElement(const char* pszKey, CCObject* pObject)
{
    CCAssert(pszKey && strlen(pszKey) > 0, "Invalid key value.");
    m_iKey = 0;
    const char* pStart = pszKey;
    
    size_t len = strlen(pszKey);
    if (len > MAX_KEY_LEN )
    {
        char* pEnd = (char*)&pszKey[len-1];
        pStart = pEnd - (MAX_KEY_LEN-1);
    }
    
    strcpy(m_szKey, pStart);
    
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::CCDictElement(intptr_t iKey, CCObject* pObject)
{
    m_szKey[0] = '\0';
    m_iKey = iKey;
    m_pObject = pObject;
    memset(&hh, 0, sizeof(hh));
}

CCDictElement::~CCDictElement()
{

}

// -----------------------------------------------------------------------
// CCDictionary

CCDictionary::CCDictionary()
: m_pElements(NULL)
, m_eDictType(kCCDictUnknown)
{

}

CCDictionary::~CCDictionary()
{
    removeAllObjects();
}

unsigned int CCDictionary::count()
{
    return HASH_COUNT(m_pElements);
}

CCArray* CCDictionary::allKeys()
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;

    CCArray* pArray = CCArray::createWithCapacity(iKeyCount);

    CCDictElement *pElement, *tmp;
    if (m_eDictType == kCCDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            CCString* pOneKey = new CCString(pElement->m_szKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            CCInteger* pOneKey = new CCInteger((int)pElement->m_iKey);
            pArray->addObject(pOneKey);
            CC_SAFE_RELEASE(pOneKey);
        }
    }
    
    return pArray;
}

CCArray* CCDictionary::allKeysForObject(CCObject* object)
{
    int iKeyCount = this->count();
    if (iKeyCount <= 0) return NULL;
    CCArray* pArray = CCArray::create();

    CCDictElement *pElement, *tmp;

    if (m_eDictType == kCCDictStr)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                CCString* pOneKey = new CCString(pElement->m_szKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    else if (m_eDictType == kCCDictInt)
    {
        HASH_ITER(hh, m_pElements, pElement, tmp) 
        {
            if (object == pElement->m_pObject)
            {
                CCInteger* pOneKey = new CCInteger((int)pElement->m_iKey);
                pArray->addObject(pOneKey);
                CC_SAFE_RELEASE(pOneKey);
            }
        }
    }
    return pArray;
}

CCObject* CCDictionary::objectForKey(const std::string& key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses string as key, therefore we should make sure that the key type of this CCDictionary is string.
    CCAssert(m_eDictType == kCCDictStr, "this dictionary does not use string as key.");

    CCObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

CCObject* CCDictionary::objectForKey(intptr_t key)
{
    // if dictionary wasn't initialized, return NULL directly.
    if (m_eDictType == kCCDictUnknown) return NULL;
    // CCDictionary only supports one kind of key, string or integer.
    // This method uses integer as key, therefore we should make sure that the key type of this CCDictionary is integer.
    CCAssert(m_eDictType == kCCDictInt, "this dictionary does not use integer as key.");

    CCObject* pRetObject = NULL;
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement != NULL)
    {
        pRetObject = pElement->m_pObject;
    }
    return pRetObject;
}

const CCString* CCDictionary::valueForKey(const std::string& key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

const CCString* CCDictionary::valueForKey(intptr_t key)
{
    CCString* pStr = dynamic_cast<CCString*>(objectForKey(key));
    if (pStr == NULL)
    {
        pStr = CCString::create("");
    }
    return pStr;
}

void CCDictionary::setObject(CCObject* pObject, const std::string& key)
{
    CCAssert(key.length() > 0 && pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictStr;
    }

    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CCObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }
}

void CCDictionary::setObject(CCObject* pObject, intptr_t key)
{
    CCAssert(pObject != NULL, "Invalid Argument!");
    if (m_eDictType == kCCDictUnknown)
    {
        m_eDictType = kCCDictInt;
    }

    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as key.");

    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    if (pElement == NULL)
    {
        setObjectUnSafe(pObject, key);
    }
    else if (pElement->m_pObject != pObject)
    {
        CCObject* pTmpObj = pElement->m_pObject;
        pTmpObj->retain();
        removeObjectForElememt(pElement);
        setObjectUnSafe(pObject, key);
        pTmpObj->release();
    }

}

void CCDictionary::removeObjectForKey(const std::string& key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictStr, "this dictionary doesn't use string as its key");
    CCAssert(key.length() > 0, "Invalid Argument!");
    CCDictElement *pElement = NULL;
    HASH_FIND_STR(m_pElements, key.c_str(), pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::removeObjectForKey(intptr_t key)
{
    if (m_eDictType == kCCDictUnknown)
    {
        return;
    }
    
    CCAssert(m_eDictType == kCCDictInt, "this dictionary doesn't use integer as its key");
    CCDictElement *pElement = NULL;
    HASH_FIND_PTR(m_pElements, &key, pElement);
    removeObjectForElememt(pElement);
}

void CCDictionary::setObjectUnSafe(CCObject* pObject, const std::string& key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key.c_str(), pObject);
    HASH_ADD_STR(m_pElements, m_szKey, pElement);
}

void CCDictionary::setObjectUnSafe(CCObject* pObject, const intptr_t key)
{
    pObject->retain();
    CCDictElement* pElement = new CCDictElement(key, pObject);
    HASH_ADD_PTR(m_pElements, m_iKey, pElement);
}

void CCDictionary::removeObjectsForKeys(CCArray* pKeyArray)
{
    CCObject* pObj = NULL;
    CCARRAY_FOREACH(pKeyArray, pObj)
    {
        CCString* pStr = (CCString*)pObj;
        removeObjectForKey(pStr->getCString());
    }
}

void CCDictionary::removeObjectForElememt(CCDictElement* pElement)
{
    if (pElement != NULL)
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);
    }
}

void CCDictionary::removeAllObjects()
{
    CCDictElement *pElement, *tmp;
    HASH_ITER(hh, m_pElements, pElement, tmp) 
    {
        HASH_DEL(m_pElements, pElement);
        pElement->m_pObject->release();
        CC_SAFE_DELETE(pElement);

    }
}

void CCDictionary::paste(CCObject* o)
{
	Super::paste(o);

	CCDictionary* pNewDict = O;

    CCDictElement* pElement = NULL;
    CCObject* pTmpObj = NULL;

    if (m_eDictType == kCCDictInt)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getIntKey());
            pTmpObj->release();
        }
    }
    else if (m_eDictType == kCCDictStr)
    {
        CCDICT_FOREACH(this, pElement)
        {
            pTmpObj = pElement->getObject()->copy();
            pNewDict->setObject(pTmpObj, pElement->getStrKey());
            pTmpObj->release();
        }
    }
}

CCObject* CCDictionary::randomObject()
{
    if (m_eDictType == kCCDictUnknown)
    {
        return NULL;
    }
    
    CCObject* key = allKeys()->randomObject();
    
    if (m_eDictType == kCCDictInt)
    {
        return objectForKey(((CCInteger*)key)->getValue());
    }
    else if (m_eDictType == kCCDictStr)
    {
        return objectForKey(((CCString*)key)->getCString());
    }
    else
    {
        return NULL;
    }
}

CCDictionary* CCDictionary::create()
{
    CCDictionary* pRet = new CCDictionary();
    if (pRet != NULL)
    {
        pRet->autorelease();
    }
    return pRet;
}

CCDictionary* CCDictionary::createWithDictionary(CCDictionary* srcDict)
{
    CCDictionary* pNewDict = (CCDictionary*)srcDict->copy();
    pNewDict->autorelease();
    return pNewDict;
}

CCDictionary* CCDictionary::createWithContentsOfFileThreadSafe(const char *pFileName)
{
    return createWithContentsOfFile(pFileName);
}

CCDictionary* CCDictionary::createWithContentsOfDataThreadSafe(const char* data, unsigned long size)
{
	CCDictMaker tMaker;
	CCDictionary* pRet = tMaker.dictionaryWithContentsOfData(data, size);
	pRet->autorelease();
	return pRet;
}

CCDictionary* CCDictionary::createWithContentsOfFile(const char *pFileName)
{
	CCDictMaker tMaker;
	CCDictionary* pRet = tMaker.dictionaryWithContentsOfFile(FileSystem::fullPathOfFile(pFileName).c_str());
    pRet->autorelease();
    return pRet;
}

CCArray* cocos2d::CCDictionary::createArrayWithContentsOfFile(const char* pFileName)
{
	CCDictMaker tMaker;
	CCArray* pRet = tMaker.arrayWithContentsOfFile(FileSystem::fullPathOfFile(pFileName).c_str());
	pRet->autorelease();
	return pRet;
}


NS_CC_END
