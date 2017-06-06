/****************************************************************************
 Copyright (c) 2013 cocos2d-x.org
 
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

#include "UICheckBox.h"
#include "nodes/CCSprite.h"

NS_CC_BEGIN

namespace ui {

CheckBox::CheckBox():
_isSelected(true),
_checkBoxEventListener(NULL),
_checkBoxEventSelector(NULL),
_backGroundFileName(""),
_backGroundSelectedFileName(""),
_frontCrossFileName(""),
_backGroundDisabledFileName(""),
_frontCrossDisabledFileName("")
{
}

CheckBox::~CheckBox()
{
    _checkBoxEventListener = NULL;
    _checkBoxEventSelector = NULL;
}

CheckBox* CheckBox::create()
{
    CheckBox* widget = new CheckBox();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool CheckBox::init()
{
    if (Widget::init())
    {
        setSelectedState(false);
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void CheckBox::initRenderer()
{
    _backGroundBoxRenderer = CCSprite::create();
    _backGroundSelectedBoxRenderer = CCSprite::create();
    _frontCrossRenderer = CCSprite::create();
    _backGroundBoxDisabledRenderer = CCSprite::create();
    _frontCrossDisabledRenderer = CCSprite::create();
}

void CheckBox::loadTextures(const char *backGround, const char *backGroundSelected, const char *cross,const char* backGroundDisabled,const char* frontCrossDisabled)
{
    loadTextureBackGround(backGround);
    loadTextureBackGroundSelected(backGroundSelected);
    loadTextureFrontCross(cross);
    loadTextureBackGroundDisabled(backGroundDisabled);
    loadTextureFrontCrossDisabled(frontCrossDisabled);
}

void CheckBox::loadTextureBackGround(const char *backGround)
{
    if (!backGround || strcmp(backGround, "") == 0)
    {
        return;
    }
    _backGroundFileName = backGround;
	_backGroundBoxRenderer->initWithFile(backGround);
    backGroundTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_backGroundBoxRenderer);
}

void CheckBox::loadTextureBackGroundSelected(const char *backGroundSelected)
{
    if (!backGroundSelected || strcmp(backGroundSelected, "") == 0)
    {
        return;
    }
    _backGroundSelectedFileName = backGroundSelected;
	_backGroundSelectedBoxRenderer->initWithFile(backGroundSelected);
	
    backGroundSelectedTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_backGroundSelectedBoxRenderer);
}

void CheckBox::loadTextureFrontCross(const char *cross)
{
    if (!cross || strcmp(cross, "") == 0)
    {
        return;
    }
    _frontCrossFileName = cross;
	_frontCrossRenderer->initWithFile(cross);
	
    frontCrossTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_frontCrossRenderer);
}

void CheckBox::loadTextureBackGroundDisabled(const char *backGroundDisabled)
{
    if (!backGroundDisabled || strcmp(backGroundDisabled, "") == 0)
    {
        return;
    }
    _backGroundDisabledFileName = backGroundDisabled;
	_backGroundBoxDisabledRenderer->initWithFile(backGroundDisabled);
	
    backGroundDisabledTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_backGroundBoxDisabledRenderer);
}

void CheckBox::loadTextureFrontCrossDisabled(const char *frontCrossDisabled)
{
    if (!frontCrossDisabled || strcmp(frontCrossDisabled, "") == 0)
    {
        return;
    }
    _frontCrossDisabledFileName = frontCrossDisabled;
	_frontCrossDisabledRenderer->initWithFile(frontCrossDisabled);
	
    frontCrossDisabledTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_frontCrossDisabledRenderer);
}

void CheckBox::onTouchEnded(CCTouch *touch, CCEvent *unused_event)
{
    _touchEndPos = touch->getLocation();
    if (_focus)
    {
        releaseUpEvent();
        if (_isSelected){
            setSelectedState(false);
            unSelectedEvent();
        }
        else
        {
            setSelectedState(true);
            selectedEvent();
        }
    }
    setFocused(false);
    Widget* widgetParent = getWidgetParent();
    if (widgetParent)
    {
        widgetParent->checkChildInfo(2,this,_touchEndPos);
    }
}

void CheckBox::onPressStateChangedToNormal()
{
    _backGroundBoxRenderer->setVisible(true);
    _backGroundSelectedBoxRenderer->setVisible(false);
    _backGroundBoxDisabledRenderer->setVisible(false);
    _frontCrossDisabledRenderer->setVisible(false);
}

void CheckBox::onPressStateChangedToPressed()
{
    _backGroundBoxRenderer->setVisible(false);
    _backGroundSelectedBoxRenderer->setVisible(true);
    _backGroundBoxDisabledRenderer->setVisible(false);
    _frontCrossDisabledRenderer->setVisible(false);
}

void CheckBox::onPressStateChangedToDisabled()
{
    _backGroundBoxRenderer->setVisible(false);
    _backGroundSelectedBoxRenderer->setVisible(false);
    _backGroundBoxDisabledRenderer->setVisible(true);
    _frontCrossRenderer->setVisible(false);
    if (_isSelected)
    {
        _frontCrossDisabledRenderer->setVisible(true);
    }
}

void CheckBox::setSelectedState(bool selected)
{
    if (selected == _isSelected)
    {
        return;
    }
    _isSelected = selected;
    _frontCrossRenderer->setVisible(_isSelected);
}

bool CheckBox::getSelectedState()
{
    return _isSelected;
}

void CheckBox::selectedEvent()
{
    if (_checkBoxEventListener && _checkBoxEventSelector)
    {
        (_checkBoxEventListener->*_checkBoxEventSelector)(this,CHECKBOX_STATE_EVENT_SELECTED);
    }
}

void CheckBox::unSelectedEvent()
{
    if (_checkBoxEventListener && _checkBoxEventSelector)
    {
        (_checkBoxEventListener->*_checkBoxEventSelector)(this,CHECKBOX_STATE_EVENT_UNSELECTED);
    }
}

void CheckBox::addEventListenerCheckBox(CCObject *target, SEL_SelectedStateEvent selector)
{
    _checkBoxEventListener = target;
    _checkBoxEventSelector = selector;
}
    
void CheckBox::updateFlippedX()
{
    _backGroundBoxRenderer->setFlipX(_flippedX);
    _backGroundSelectedBoxRenderer->setFlipX(_flippedX);
    _frontCrossRenderer->setFlipX(_flippedX);
    _backGroundBoxDisabledRenderer->setFlipX(_flippedX);
    _frontCrossDisabledRenderer->setFlipX(_flippedX);
}
    
void CheckBox::updateFlippedY()
{
    _backGroundBoxRenderer->setFlipY(_flippedY);
    _backGroundSelectedBoxRenderer->setFlipY(_flippedY);
    _frontCrossRenderer->setFlipY(_flippedY);
    _backGroundBoxDisabledRenderer->setFlipY(_flippedY);
    _frontCrossDisabledRenderer->setFlipY(_flippedY);
}

void CheckBox::setAnchorPoint(const Vec2 &pt)
{
    Widget::setAnchorPoint(pt);
    _backGroundBoxRenderer->setAnchorPoint(pt);
    _backGroundSelectedBoxRenderer->setAnchorPoint(pt);
    _backGroundBoxDisabledRenderer->setAnchorPoint(pt);
    _frontCrossRenderer->setAnchorPoint(pt);
    _frontCrossDisabledRenderer->setAnchorPoint(pt);
}

void CheckBox::onSizeChanged()
{
    Widget::onSizeChanged();
    backGroundTextureScaleChangedWithSize();
    backGroundSelectedTextureScaleChangedWithSize();
    frontCrossTextureScaleChangedWithSize();
    backGroundDisabledTextureScaleChangedWithSize();
    frontCrossDisabledTextureScaleChangedWithSize();
}

const CCSize& CheckBox::getSize() const
{
    return _backGroundBoxRenderer->getSize();
}

CCNode* CheckBox::getVirtualRenderer()
{
    return _backGroundBoxRenderer;
}

void CheckBox::draw()
{
	_backGroundBoxRenderer->draw();
	_backGroundSelectedBoxRenderer->visit();
	_frontCrossRenderer->visit();
	_backGroundBoxDisabledRenderer->visit();
	_frontCrossDisabledRenderer->visit();
}

void CheckBox::backGroundTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundBoxRenderer->setScale(1.0f);
        _size = _backGroundBoxRenderer->getSize();
    }
    else
    {
        CCSize textureSize = _backGroundBoxRenderer->getSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundBoxRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundBoxRenderer->setScaleX(scaleX);
        _backGroundBoxRenderer->setScaleY(scaleY);
    }
}

void CheckBox::backGroundSelectedTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundSelectedBoxRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _backGroundSelectedBoxRenderer->getSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundSelectedBoxRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundSelectedBoxRenderer->setScaleX(scaleX);
        _backGroundSelectedBoxRenderer->setScaleY(scaleY);
    }
}

void CheckBox::frontCrossTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _frontCrossRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _frontCrossRenderer->getSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _frontCrossRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _frontCrossRenderer->setScaleX(scaleX);
        _frontCrossRenderer->setScaleY(scaleY);
    }
}

void CheckBox::backGroundDisabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _backGroundBoxDisabledRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _backGroundBoxDisabledRenderer->getSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _backGroundBoxDisabledRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _backGroundBoxDisabledRenderer->setScaleX(scaleX);
        _backGroundBoxDisabledRenderer->setScaleY(scaleY);
    }
}

void CheckBox::frontCrossDisabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        _frontCrossDisabledRenderer->setScale(1.0f);
    }
    else
    {
        CCSize textureSize = _frontCrossDisabledRenderer->getSize();
        if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
        {
            _frontCrossDisabledRenderer->setScale(1.0f);
            return;
        }
        float scaleX = _size.width / textureSize.width;
        float scaleY = _size.height / textureSize.height;
        _frontCrossDisabledRenderer->setScaleX(scaleX);
        _frontCrossDisabledRenderer->setScaleY(scaleY);
    }
}
    
void CheckBox::updateTextureColor()
{
    updateColorToRenderer(_backGroundBoxRenderer);
    updateColorToRenderer(_backGroundSelectedBoxRenderer);
    updateColorToRenderer(_frontCrossRenderer);
    updateColorToRenderer(_backGroundBoxDisabledRenderer);
    updateColorToRenderer(_frontCrossDisabledRenderer);
}

void CheckBox::updateTextureOpacity()
{
    updateOpacityToRenderer(_backGroundBoxRenderer);
    updateOpacityToRenderer(_backGroundSelectedBoxRenderer);
    updateOpacityToRenderer(_frontCrossRenderer);
    updateOpacityToRenderer(_backGroundBoxDisabledRenderer);
    updateOpacityToRenderer(_frontCrossDisabledRenderer);
}

void CheckBox::updateTextureRGBA()
{
    updateRGBAToRenderer(_backGroundBoxRenderer);
    updateRGBAToRenderer(_backGroundSelectedBoxRenderer);
    updateRGBAToRenderer(_frontCrossRenderer);
    updateRGBAToRenderer(_backGroundBoxDisabledRenderer);
    updateRGBAToRenderer(_frontCrossDisabledRenderer);
}

std::string CheckBox::getDescription() const
{
    return "CheckBox";
}

Widget* CheckBox::createCloneInstance()
{
    return CheckBox::create();
}

void CheckBox::copySpecialProperties(Widget *widget)
{
    CheckBox* checkBox = dynamic_cast<CheckBox*>(widget);
    if (checkBox)
    {
        loadTextureBackGround(checkBox->_backGroundFileName.c_str());
        loadTextureBackGroundSelected(checkBox->_backGroundSelectedFileName.c_str());
        loadTextureFrontCross(checkBox->_frontCrossFileName.c_str());
        loadTextureBackGroundDisabled(checkBox->_backGroundDisabledFileName.c_str());
        loadTextureFrontCrossDisabled(checkBox->_frontCrossDisabledFileName.c_str());
        setSelectedState(checkBox->_isSelected);
    }
}

}

NS_CC_END
