/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2012 James Chen
 
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

#include "UIEditBox.h"
#include "UIEditBoxImpl.h"
#include "textures/CCTexture2D.h"
#include "resources/CCSpriteFrame.h"

NS_CC_BEGIN
namespace ui {

EditBox::EditBox(void)
: m_pEditBoxImpl(NULL)
, m_pDelegate(NULL)
, m_eEditBoxInputMode(kEditBoxInputModeSingleLine)
, m_eEditBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, m_eKeyboardReturnType(kKeyboardReturnTypeDefault)
, m_nFontSize(-1)
, m_nPlaceholderFontSize(-1)
, m_colText(ccWHITE)
, m_colPlaceHolder(ccGRAY)
, m_nMaxLength(0)
, m_fAdjustHeight(0.0f)
, m_nScriptEditBoxHandler(0)
{
}

EditBox::~EditBox(void)
{
    CC_SAFE_DELETE(m_pEditBoxImpl);
    unregisterScriptEditBoxHandler();
}
bool EditBox::init()
{
    if (BaseWidget::init())
    {
        CCSize dfSize(100, 50);
        m_pEditBoxImpl = __createSystemEditBox(this);
        m_pEditBoxImpl->initWithSize(dfSize);
        setSize(dfSize);
        ignoreContentAdaptWithSize(false);
        
        return true;
    }
    
    return false;
}

void EditBox::onPressStateChangedToPressed()
{
	m_pEditBoxImpl->openKeyboard();
}

EditBox* EditBox::create()
{
	EditBox* pRet = new EditBox();
	if (pRet && pRet->init())
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return nullptr;
}

void EditBox::setDelegate(CCEditBoxDelegate* pDelegate)
{
    m_pDelegate = pDelegate;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setDelegate(pDelegate);
    }
}

CCEditBoxDelegate* EditBox::getDelegate()
{
    return m_pDelegate;
}

void EditBox::setText(const char* pText)
{
    if (pText != NULL)
    {
        m_strText = pText;
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setText(pText);
        }
    }
}

const char* EditBox::getText(void)
{
    if (m_pEditBoxImpl != NULL)
    {
		const char* pText = m_pEditBoxImpl->getText();
		if(pText != NULL)
			return pText;
    }
    
    return "";
}

void EditBox::setFont(const char* pFontName, int fontSize)
{
    m_strFontName = pFontName;
    m_nFontSize = fontSize;
    if (pFontName != NULL)
    {
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setFont(pFontName, fontSize);
        }
    }
}

void EditBox::setFontName(const char* pFontName)
{
    m_strFontName = pFontName;
    if (m_pEditBoxImpl != NULL && m_nFontSize != -1)
    {
        m_pEditBoxImpl->setFont(pFontName, m_nFontSize);
    }
}

void EditBox::setFontSize(int fontSize)
{
    m_nFontSize = fontSize;
    if (m_pEditBoxImpl != NULL && m_strFontName.length() > 0)
    {
        m_pEditBoxImpl->setFont(m_strFontName.c_str(), m_nFontSize);
    }
}

void EditBox::setFontColor(const ccColor3B& color)
{
    m_colText = color;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setFontColor(color);
    }
}

void EditBox::setPlaceholderFont(const char* pFontName, int fontSize)
{
    m_strPlaceholderFontName = pFontName;
    m_nPlaceholderFontSize = fontSize;
    if (pFontName != NULL)
    {
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setPlaceholderFont(pFontName, fontSize);
        }
    }
}

void EditBox::setPlaceholderFontName(const char* pFontName)
{
    m_strPlaceholderFontName = pFontName;
    if (m_pEditBoxImpl != NULL && m_nPlaceholderFontSize != -1)
    {
        m_pEditBoxImpl->setPlaceholderFont(pFontName, m_nFontSize);
    }
}

void EditBox::setPlaceholderFontSize(int fontSize)
{
    m_nPlaceholderFontSize = fontSize;
    if (m_pEditBoxImpl != NULL && m_strPlaceholderFontName.length() > 0)
    {
        m_pEditBoxImpl->setPlaceholderFont(m_strPlaceholderFontName.c_str(), m_nFontSize);
    }
}

void EditBox::setPlaceholderFontColor(const ccColor3B& color)
{
    m_colText = color;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setPlaceholderFontColor(color);
    }
}

void EditBox::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        m_strPlaceHolder = pText;
        if (m_pEditBoxImpl != NULL)
        {
            m_pEditBoxImpl->setPlaceHolder(pText);
        }
    }
}

const char* EditBox::getPlaceHolder(void)
{
    return m_strPlaceHolder.c_str();
}

void EditBox::setInputMode(EditBoxInputMode inputMode)
{
    m_eEditBoxInputMode = inputMode;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setInputMode(inputMode);
    }
}

void EditBox::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setMaxLength(maxLength);
    }
}


int EditBox::getMaxLength()
{
    return m_nMaxLength;
}

void EditBox::setInputFlag(EditBoxInputFlag inputFlag)
{
    m_eEditBoxInputFlag = inputFlag;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setInputFlag(inputFlag);
    }
}

void EditBox::setReturnType(KeyboardReturnType returnType)
{
    m_eKeyboardReturnType = returnType;
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setReturnType(returnType);
    }
}

KeyboardReturnType EditBox::getReturnType()
{
    return m_eKeyboardReturnType;
}

/* override function */
void EditBox::setPosition(const CCPoint& pos)
{
	BaseWidget::setPosition(pos);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setPosition(pos);
    }
}

void EditBox::setVisible(bool visible)
{
	BaseWidget::setVisible(visible);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setVisible(visible);
    }
}

void EditBox::setSize(const CCSize& size)
{
	BaseWidget::setSize(size);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setContentSize(size);
    }
}

void EditBox::setAnchorPoint(const CCPoint& anchorPoint)
{
	BaseWidget::setAnchorPoint(anchorPoint);
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->setAnchorPoint(anchorPoint);
    }
}

void EditBox::visit(void)
{
	BaseWidget::visit();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->visit();
    }
}

void EditBox::onEnter(void)
{
	BaseWidget::onEnter();
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->onEnter();
    }
}

void EditBox::onExit(void)
{
	BaseWidget::onExit();
    if (m_pEditBoxImpl != NULL)
    {
        // remove system edit control
        m_pEditBoxImpl->closeKeyboard();
    }
}

void EditBox::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
{
    CCSize contentSize = this->getSize();
    float left = -contentSize.width * m_obAnchorPoint.x;
    float bottom = contentSize.height * (0.5 - m_obAnchorPoint.y) - contentSize.height/2;

    CCRect rect = CCRectMake(left, bottom, contentSize.width, contentSize.height);
    CCRect rectTracked = CCRectApplyAffineTransform(rect, this->nodeToWorldTransform());

	// some adjustment for margin between the keyboard and the edit box.
	rectTracked.origin.y -= 4;

    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
    if (!rectTracked.intersectsRect(info.end))
    {
        CCLOG("needn't to adjust view layout.");
        return;
    }
    
    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
    m_fAdjustHeight = info.end.getMaxY() - rectTracked.getMinY();
    // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", m_fAdjustHeight);
    
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, m_fAdjustHeight);
    }
}

void EditBox::keyboardDidShow(CCIMEKeyboardNotificationInfo& info)
{
	
}

void EditBox::keyboardWillHide(CCIMEKeyboardNotificationInfo& info)
{
    // CCLOG("CCEditBox::keyboardWillHide");
    if (m_pEditBoxImpl != NULL)
    {
        m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, -m_fAdjustHeight);
    }
}

void EditBox::keyboardDidHide(CCIMEKeyboardNotificationInfo& info)
{
	
}

void EditBox::registerScriptEditBoxHandler(int handler)
{
    unregisterScriptEditBoxHandler();
    m_nScriptEditBoxHandler = handler;
}

void EditBox::unregisterScriptEditBoxHandler(void)
{
    if (0 != m_nScriptEditBoxHandler)
    {
        CCScriptEngineManager::sharedManager()->getScriptEngine()->removeScriptHandler(m_nScriptEditBoxHandler);
        m_nScriptEditBoxHandler = 0;
    }
}

}
NS_CC_END