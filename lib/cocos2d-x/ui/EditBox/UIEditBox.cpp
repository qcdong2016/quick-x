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
#include "textures/CCTexture2D.h"
#include "resources/CCSpriteFrame.h"
#include "cocoa/ccUTF8.h"


NS_CC_BEGIN

void openKeyboard(ui::EditBox*);
void closeKeyboard(ui::EditBox*);


namespace ui {
    

EditBox::EditBox(void)
: m_eEditBoxInputMode(kEditBoxInputModeSingleLine)
, m_eEditBoxInputFlag(kEditBoxInputFlagInitialCapsAllCharacters)
, m_eKeyboardReturnType(kKeyboardReturnTypeDefault)
, m_nMaxLength(0)
, m_fAdjustHeight(0.0f)
{
}

EditBox::~EditBox(void)
{
#if BUILD_FOR(IOS)
    closeKeyboard(this);
#endif
}
bool EditBox::init()
{
    if (BaseWidget::init())
    {
        setSize(CCSize(100, 50));
        ignoreContentAdaptWithSize(false);
        setTouchEnabled(true);
        
        return true;
    }

    return false;
}

void EditBox::initRenderer()
{
    _labelRender = CCLabelTTF::create();
    _labelRender->setColor(ccWHITE);
    
    _labelPlaceHolderRender = CCLabelTTF::create();
    _labelPlaceHolderRender->setColor(ccGRAY);
    setPlaceHolder("input text");
}

void EditBox::onPressStateChangedToPressed()
{
#if BUILD_FOR(IOS)
    openKeyboard(this);
#endif
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

void EditBox::setText(const char* pText)
{
    if (pText)
    {
        _text = pText;
        if (kEditBoxInputFlagPassword == m_eEditBoxInputFlag)
        {
            std::string strToShow;
            long length = cc_utf8_strlen(pText);
            for (long i = 0; i < length; i++)
            {
                strToShow.append("\u25CF");
            }
            _labelRender->setString(strToShow.c_str());
        }
        else
        {
            _labelRender->setString(pText);
        }
        _labelRender->setTextureRect(CCRect(0, 0, getSize().width, _labelRender->getTextureRect().size.height));
        
        if (*pText == '\0')
        {
            _text = "";
            return;
        }
    }
    else
    {
        _text = "";
    }
}

const char* EditBox::getText(void)
{
    return _text.c_str();
}
    
void EditBox::onSizeChanged()
{
    if (_size.width == 0.0f || _size.height == 0.0f)
        return;

    float left = - _size.width * m_obAnchorPoint.x;
    
    _labelRender->setFontSize(_size.height * 0.75);
    _labelRender->setAnchorPoint(Vec2(0, 0.5));
    _labelRender->setTextureRect(CCRect(0, 0, _size.width, _size.height));
    _labelRender->setPositionX(left);
    
    _labelPlaceHolderRender->setFontSize(_size.height * 0.75);
    _labelPlaceHolderRender->setAnchorPoint(Vec2(0, 0.5));
    _labelPlaceHolderRender->setTextureRect(CCRect(0, 0, _size.width, _size.height));
    _labelPlaceHolderRender->setPositionX(left);
}

void EditBox::setFontName(const char* pFontName)
{
    _labelRender->setFontName(pFontName);
}

const char* EditBox::getFontName()
{
    return _labelRender->getFontName();
}

void EditBox::setFontSize(int fontSize)
{
    _labelRender->setFontSize(fontSize);
}

int EditBox::getFontSize()
{
    return _labelRender->getFontSize();
}

void EditBox::setFontColor(const ccColor3B& color)
{
    _labelRender->setFontFillColor(color);
}

void EditBox::setPlaceholderFontName(const char* pFontName)
{
    _labelPlaceHolderRender->setFontName(pFontName);
}

void EditBox::setPlaceholderFontSize(int fontSize)
{
    _labelPlaceHolderRender->setFontSize(fontSize);
}

void EditBox::setPlaceholderFontColor(const ccColor3B& color)
{
    _labelPlaceHolderRender->setFontFillColor(color);
}

void EditBox::setPlaceHolder(const char* pText)
{
    if (pText != NULL)
    {
        _labelPlaceHolderRender->setString(pText);
    }
}

const char* EditBox::getPlaceHolder(void)
{
    return _labelPlaceHolderRender->getString();
}

void EditBox::setInputMode(EditBoxInputMode inputMode)
{
    m_eEditBoxInputMode = inputMode;
}

EditBoxInputMode EditBox::getInputMode()
{
    return m_eEditBoxInputMode;
}

void EditBox::setMaxLength(int maxLength)
{
    m_nMaxLength = maxLength;
}


int EditBox::getMaxLength()
{
    return m_nMaxLength;
}

void EditBox::setInputFlag(EditBoxInputFlag inputFlag)
{
    m_eEditBoxInputFlag = inputFlag;
}

EditBoxInputFlag EditBox::getInputFlag()
{
    return m_eEditBoxInputFlag;
}

void EditBox::setReturnType(KeyboardReturnType returnType)
{
    m_eKeyboardReturnType = returnType;
}

KeyboardReturnType EditBox::getReturnType()
{
    return m_eKeyboardReturnType;
}

void EditBox::setAnchorPoint(const Vec2& anchorPoint)
{
	BaseWidget::setAnchorPoint(anchorPoint);
    _labelPlaceHolderRender->setAnchorPoint(anchorPoint);
    _labelRender->setAnchorPoint(anchorPoint);
}

void EditBox::draw()
{
    if (_text != "")
        _labelRender->visit();
    else
        _labelPlaceHolderRender->visit();
}

//
//void EditBox::keyboardWillShow(CCIMEKeyboardNotificationInfo& info)
//{
//    CCSize contentSize = this->getSize();
//    float left = -contentSize.width * m_obAnchorPoint.x;
//    float bottom = contentSize.height * (0.5 - m_obAnchorPoint.y) - contentSize.height/2;
//
//    CCRect rect = CCRectMake(left, bottom, contentSize.width, contentSize.height);
//    CCRect rectTracked = CCRectApplyAffineTransform(rect, this->nodeToWorldTransform());
//
//	// some adjustment for margin between the keyboard and the edit box.
//    rectTracked.origin.y -= 4;
//
//    // if the keyboard area doesn't intersect with the tracking node area, nothing needs to be done.
//    if (!rectTracked.intersectsRect(info.end))
//    {
//        CCLOG("needn't to adjust view layout.");
//        return;
//    }
//    
//    // assume keyboard at the bottom of screen, calculate the vertical adjustment.
//    m_fAdjustHeight = info.end.getMaxY() - rectTracked.getMinY();
//    // CCLOG("CCEditBox:needAdjustVerticalPosition(%f)", m_fAdjustHeight);
//    
//    m_pEditBoxImpl->doAnimationWhenKeyboardMove(info.duration, m_fAdjustHeight);
//}

}
NS_CC_END
