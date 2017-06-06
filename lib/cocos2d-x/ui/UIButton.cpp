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

#include "UIButton.h"

#include "nodes/CCSprite.h"
#include "nodes/CCScale9Sprite.h"
#include "nodes/CCLabelTTF.h"
#include "actions/CCActionInterval.h"

NS_CC_BEGIN

namespace ui {

Button::Button():
_normalFileName(""),
_clickedFileName(""),
_disabledFileName(""),
_prevIgnoreSize(true),
_scale9Enabled(false),
_capInsetsNormal(CCRectZero),
_capInsetsPressed(CCRectZero),
_capInsetsDisabled(CCRectZero),
_normalTextureSize(_size),
_pressedTextureSize(_size),
_disabledTextureSize(_size),
_pressedActionEnabled(false),
_titleColor(ccWHITE),
_normalTextureScaleXInSize(1.0f),
_normalTextureScaleYInSize(1.0f),
_pressedTextureScaleXInSize(1.0f),
_pressedTextureScaleYInSize(1.0f),
_normalTextureLoaded(false),
_pressedTextureLoaded(false),
_disabledTextureLoaded(false)
{
    
}

Button::~Button()
{
}

Button* Button::create()
{
    Button* widget = new Button();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return NULL;
}

bool Button::init()
{
    if (Widget::init())
    {
        setTouchEnabled(true);
        return true;
    }
    return false;
}

void Button::initRenderer()
{
    _buttonNormalRenderer = CCSprite::create();
    _buttonClickedRenderer = CCSprite::create();
    _buttonDisableRenderer = CCSprite::create();
}

void Button::setScale9Enabled(bool able)
{
    if (_scale9Enabled == able)
    {
        return;
    }
    _brightStyle = BRIGHT_NONE;
    _scale9Enabled = able;

    if (_scale9Enabled)
    {
        _buttonNormalRenderer = CCScale9Sprite::create();
        _buttonClickedRenderer = CCScale9Sprite::create();
        _buttonDisableRenderer = CCScale9Sprite::create();
    }
    else
    {
        _buttonNormalRenderer = CCSprite::create();
        _buttonClickedRenderer = CCSprite::create();
        _buttonDisableRenderer = CCSprite::create();
    }

    loadTextureNormal(_normalFileName.c_str());
    loadTexturePressed(_clickedFileName.c_str());
    loadTextureDisabled(_disabledFileName.c_str());

    if (_scale9Enabled)
    {
        bool ignoreBefore = _ignoreSize;
        ignoreContentAdaptWithSize(false);
        _prevIgnoreSize = ignoreBefore;
    }
    else
    {
        ignoreContentAdaptWithSize(_prevIgnoreSize);
    }
    setCapInsetsNormalRenderer(_capInsetsNormal);
    setCapInsetsPressedRenderer(_capInsetsPressed);
    setCapInsetsDisabledRenderer(_capInsetsDisabled);
    setBright(_bright);
}
    
bool Button::isScale9Enabled()
{
    return _scale9Enabled;
}

void Button::ignoreContentAdaptWithSize(bool ignore)
{
    if (!_scale9Enabled || (_scale9Enabled && !ignore))
    {
        Widget::ignoreContentAdaptWithSize(ignore);
        _prevIgnoreSize = ignore;
    }
}

void Button::loadTextures(const char* normal,const char* selected,const char* disabled)
{
    loadTextureNormal(normal);
    loadTexturePressed(selected);
    loadTextureDisabled(disabled);
}

void Button::loadTextureNormal(const char* normal)
{
    if (!normal || strcmp(normal, "") == 0)
    {
        return;
    }
    _normalFileName = normal;
    if (_scale9Enabled)
    {
		CCScale9Sprite* normalRendererScale9 = static_cast<CCScale9Sprite*>(_buttonDisableRenderer.Get());
		normalRendererScale9->initWithFile(normal);
		
        normalRendererScale9->setCapInsets(_capInsetsNormal);
    }
    else
    {
        CCSprite* normalRenderer = static_cast<CCSprite*>(_buttonNormalRenderer.Get());
		normalRenderer->initWithFile(normal);
    }
    _normalTextureSize = _buttonNormalRenderer->getSize();
    normalTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_buttonNormalRenderer);
    _normalTextureLoaded = true;
}

void Button::loadTexturePressed(const char* selected)
{
    if (!selected || strcmp(selected, "") == 0)
    {
        return;
    }
    _clickedFileName = selected;
    if (_scale9Enabled)
    {
        CCScale9Sprite* clickedRendererScale9 = static_cast<CCScale9Sprite*>(_buttonClickedRenderer.Get());
		clickedRendererScale9->initWithFile(selected);
        clickedRendererScale9->setCapInsets(_capInsetsPressed);
    }
    else
    {
		CCSprite* clickedRenderer = static_cast<CCSprite*>(_buttonClickedRenderer.Get());
		clickedRenderer->initWithFile(selected);
		
    }
    _pressedTextureSize = _buttonClickedRenderer->getSize();
    pressedTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_buttonClickedRenderer);
    _pressedTextureLoaded = true;
}

void Button::loadTextureDisabled(const char* disabled)
{
    if (!disabled || strcmp(disabled, "") == 0)
    {
        return;
    }
    _disabledFileName = disabled;
    if (_scale9Enabled)
    {
        CCScale9Sprite* disabledScale9 = static_cast<CCScale9Sprite*>(_buttonDisableRenderer.Get());
		disabledScale9->initWithFile(disabled);
        disabledScale9->setCapInsets(_capInsetsDisabled);
    }
    else
    {
        CCSprite* disabledRenderer = static_cast<CCSprite*>(_buttonDisableRenderer.Get());
		disabledRenderer->initWithFile(disabled);
		
    }
    _disabledTextureSize = _buttonDisableRenderer->getSize();
    disabledTextureScaleChangedWithSize();
    updateAnchorPoint();
    updateFlippedX();
    updateFlippedY();
    updateRGBAToRenderer(_buttonDisableRenderer);
    _disabledTextureLoaded = true;
}

void Button::setCapInsets(const CCRect &capInsets)
{
    setCapInsetsNormalRenderer(capInsets);
    setCapInsetsPressedRenderer(capInsets);
    setCapInsetsDisabledRenderer(capInsets);
}

void Button::setCapInsetsNormalRenderer(const CCRect &capInsets)
{
    _capInsetsNormal = capInsets;
    if (!_scale9Enabled)
    {
        return;
    }
    static_cast<CCScale9Sprite*>(_buttonNormalRenderer.Get())->setCapInsets(capInsets);
}
    
const CCRect& Button::getCapInsetNormalRenderer()
{
    return _capInsetsNormal;
}

void Button::setCapInsetsPressedRenderer(const CCRect &capInsets)
{
    _capInsetsPressed = capInsets;
    if (!_scale9Enabled)
    {
        return;
    }
    static_cast<CCScale9Sprite*>(_buttonClickedRenderer.Get())->setCapInsets(capInsets);
}
    
const CCRect& Button::getCapInsetPressedRenderer()
{
    return _capInsetsPressed;
}

void Button::setCapInsetsDisabledRenderer(const CCRect &capInsets)
{
    _capInsetsDisabled = capInsets;
    if (!_scale9Enabled)
    {
        return;
    }
    static_cast<CCScale9Sprite*>(_buttonDisableRenderer.Get())->setCapInsets(capInsets);
}
    
const CCRect& Button::getCapInsetDisabledRenderer()
{
    return _capInsetsDisabled;
}

void Button::onPressStateChangedToNormal()
{
    _buttonNormalRenderer->setVisible(true);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisableRenderer->setVisible(false);
    if (_pressedTextureLoaded)
    {
        if (_pressedActionEnabled)
        {
            _buttonNormalRenderer->stopAllActions();
            _buttonClickedRenderer->stopAllActions();
            CCAction *zoomAction = CCScaleTo::create(0.05f, _normalTextureScaleXInSize, _normalTextureScaleYInSize);
            _buttonNormalRenderer->runAction(zoomAction);
            _buttonClickedRenderer->setScale(_pressedTextureScaleXInSize, _pressedTextureScaleYInSize);
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            updateTextureRGBA();
        }
        else
        {
            _buttonNormalRenderer->stopAllActions();
            _buttonNormalRenderer->setScale(_normalTextureScaleXInSize, _normalTextureScaleYInSize);
        }
    }
}

void Button::onPressStateChangedToPressed()
{
    if (_pressedTextureLoaded)
    {
        _buttonNormalRenderer->setVisible(false);
        _buttonClickedRenderer->setVisible(true);
        _buttonDisableRenderer->setVisible(false);
        if (_pressedActionEnabled)
        {
            _buttonNormalRenderer->stopAllActions();
            _buttonClickedRenderer->stopAllActions();
            CCAction *zoomAction = CCScaleTo::create(0.05f, _pressedTextureScaleXInSize + 0.1f, _pressedTextureScaleYInSize + 0.1f);
            _buttonClickedRenderer->runAction(zoomAction);
            _buttonNormalRenderer->setScale(_pressedTextureScaleXInSize + 0.1f, _pressedTextureScaleYInSize + 0.1f);
        }
    }
    else
    {
        _buttonNormalRenderer->setVisible(true);
        _buttonClickedRenderer->setVisible(true);
        _buttonDisableRenderer->setVisible(false);
        if (_scale9Enabled)
        {
            _buttonNormalRenderer->setColor(ccGRAY);
        }
        else
        {
            _buttonNormalRenderer->stopAllActions();
            _buttonNormalRenderer->setScale(_normalTextureScaleXInSize + 0.1f, _normalTextureScaleYInSize + 0.1f);
        }
    }
}

void Button::onPressStateChangedToDisabled()
{
    _buttonNormalRenderer->setVisible(false);
    _buttonClickedRenderer->setVisible(false);
    _buttonDisableRenderer->setVisible(true);
    _buttonNormalRenderer->setScale(_normalTextureScaleXInSize, _normalTextureScaleYInSize);
    _buttonClickedRenderer->setScale(_pressedTextureScaleXInSize, _pressedTextureScaleYInSize);
}
    
void Button::updateFlippedX()
{
    if (_scale9Enabled)
    {
        if (_flippedX)
        {
            _buttonNormalRenderer->setScaleX(-1);
            _buttonClickedRenderer->setScaleX(-1);
            _buttonDisableRenderer->setScaleX(-1);
        }
        else
        {
            _buttonNormalRenderer->setScaleX(1);
            _buttonClickedRenderer->setScaleX(1);
            _buttonDisableRenderer->setScaleX(1);
        }
        
    }
    else
    {
        static_cast<CCSprite*>(_buttonNormalRenderer.Get())->setFlipX(_flippedX);
        static_cast<CCSprite*>(_buttonClickedRenderer.Get())->setFlipX(_flippedX);
        static_cast<CCSprite*>(_buttonDisableRenderer.Get())->setFlipX(_flippedX);
    }
}
    
void Button::updateFlippedY()
{
    if (_scale9Enabled)
    {
        if (_flippedY)
        {
            _buttonNormalRenderer->setScaleY(-1);
            _buttonClickedRenderer->setScaleY(-1);
            _buttonDisableRenderer->setScaleY(-1);
        }
        else
        {
            _buttonNormalRenderer->setScaleY(1);
            _buttonClickedRenderer->setScaleY(1);
            _buttonDisableRenderer->setScaleY(1);
        }

    }
    else
    {
        static_cast<CCSprite*>(_buttonNormalRenderer.Get())->setFlipY(_flippedY);
        static_cast<CCSprite*>(_buttonClickedRenderer.Get())->setFlipY(_flippedY);
        static_cast<CCSprite*>(_buttonDisableRenderer.Get())->setFlipY(_flippedY);
    }
}

void Button::setAnchorPoint(const Vec2 &pt)
{
    Widget::setAnchorPoint(pt);
    _buttonNormalRenderer->setAnchorPoint(pt);
    _buttonClickedRenderer->setAnchorPoint(pt);
    _buttonDisableRenderer->setAnchorPoint(pt);
}

void Button::onSizeChanged()
{
    Widget::onSizeChanged();
    normalTextureScaleChangedWithSize();
    pressedTextureScaleChangedWithSize();
    disabledTextureScaleChangedWithSize();
}

const CCSize& Button::getSize() const
{
    return _normalTextureSize;
}

void Button::draw()
{
	_buttonNormalRenderer->visit();
	_buttonClickedRenderer->visit();
	_buttonDisableRenderer->visit();
}

CCNode* Button::getVirtualRenderer()
{
    if (_bright)
    {
        switch (_brightStyle)
        {
            case BRIGHT_NORMAL:
                return _buttonNormalRenderer;
            case BRIGHT_HIGHLIGHT:
                return _buttonClickedRenderer;
            default:
                return NULL;
        }
    }
    else
    {
        return _buttonDisableRenderer;
    }
}

void Button::normalTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!_scale9Enabled)
        {
            _buttonNormalRenderer->setScale(1.0f);
            _normalTextureScaleXInSize = _normalTextureScaleYInSize = 1.0f;
            _size = _normalTextureSize;
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            static_cast<CCScale9Sprite*>(_buttonNormalRenderer.Get())->setPreferredSize(_size);
            _normalTextureScaleXInSize = _normalTextureScaleYInSize = 1.0f;
        }
        else
        {
            CCSize textureSize = _normalTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                _buttonNormalRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / textureSize.width;
            float scaleY = _size.height / textureSize.height;
            _buttonNormalRenderer->setScaleX(scaleX);
            _buttonNormalRenderer->setScaleY(scaleY);
            _normalTextureScaleXInSize = scaleX;
            _normalTextureScaleYInSize = scaleY;
        }
    }
}

void Button::pressedTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!_scale9Enabled)
        {
            _buttonClickedRenderer->setScale(1.0f);
            _pressedTextureScaleXInSize = _pressedTextureScaleYInSize = 1.0f;
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            static_cast<CCScale9Sprite*>(_buttonClickedRenderer.Get())->setPreferredSize(_size);
            _pressedTextureScaleXInSize = _pressedTextureScaleYInSize = 1.0f;
        }
        else
        {
            CCSize textureSize = _pressedTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                _buttonClickedRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / _pressedTextureSize.width;
            float scaleY = _size.height / _pressedTextureSize.height;
            _buttonClickedRenderer->setScaleX(scaleX);
            _buttonClickedRenderer->setScaleY(scaleY);
            _pressedTextureScaleXInSize = scaleX;
            _pressedTextureScaleYInSize = scaleY;
        }
    }
}

void Button::disabledTextureScaleChangedWithSize()
{
    if (_ignoreSize)
    {
        if (!_scale9Enabled)
        {
            _buttonDisableRenderer->setScale(1.0f);
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            static_cast<CCScale9Sprite*>(_buttonDisableRenderer.Get())->setPreferredSize(_size);
        }
        else
        {
            CCSize textureSize = _disabledTextureSize;
            if (textureSize.width <= 0.0f || textureSize.height <= 0.0f)
            {
                _buttonDisableRenderer->setScale(1.0f);
                return;
            }
            float scaleX = _size.width / _disabledTextureSize.width;
            float scaleY = _size.height / _disabledTextureSize.height;
            _buttonDisableRenderer->setScaleX(scaleX);
            _buttonDisableRenderer->setScaleY(scaleY);
        }
    }
}

void Button::setPressedActionEnabled(bool enabled)
{
    _pressedActionEnabled = enabled;
}

void Button::updateTextureColor()
{
    updateColorToRenderer(_buttonNormalRenderer);
    updateColorToRenderer(_buttonClickedRenderer);
    updateColorToRenderer(_buttonDisableRenderer);
}
    
void Button::updateTextureOpacity()
{
    updateOpacityToRenderer(_buttonNormalRenderer);
    updateOpacityToRenderer(_buttonClickedRenderer);
    updateOpacityToRenderer(_buttonDisableRenderer);
}
    
void Button::updateTextureRGBA()
{
    updateRGBAToRenderer(_buttonNormalRenderer);
    updateRGBAToRenderer(_buttonClickedRenderer);
    updateRGBAToRenderer(_buttonDisableRenderer);
}

std::string Button::getDescription() const
{
    return "Button";
}

Widget* Button::createCloneInstance()
{
    return Button::create();
}

void Button::copySpecialProperties(Widget *widget)
{
    Button* button = dynamic_cast<Button*>(widget);
    if (button)
    {   
        _prevIgnoreSize = button->_prevIgnoreSize;
        setScale9Enabled(button->_scale9Enabled);
        loadTextureNormal(button->_normalFileName.c_str());
        loadTexturePressed(button->_clickedFileName.c_str());
        loadTextureDisabled(button->_disabledFileName.c_str());
        setCapInsetsNormalRenderer(button->_capInsetsNormal);
        setCapInsetsPressedRenderer(button->_capInsetsPressed);
        setCapInsetsDisabledRenderer(button->_capInsetsDisabled);
        setPressedActionEnabled(button->_pressedActionEnabled);
    }
}

}

NS_CC_END
