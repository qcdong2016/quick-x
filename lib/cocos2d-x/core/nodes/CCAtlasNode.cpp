/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2008-2010 Ricardo Quesada
Copyright (c) 2011      Zynga Inc.
 
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

#include "CCAtlasNode.h"
#include "textures/CCTextureAtlas.h"

#include "engine/CCDirector.h"
#include "shaders/CCGLProgram.h"
#include "shaders/CCShaderCache.h"
#include "shaders/ccGLStateCache.h"
#include "engine/CCDirector.h"
#include "cocoa/TransformUtils.h"

// external
#include "kazmath/matrix.h"
#include "engine/CCResourceCache.h"
#include "textures/CCTexture2D.h"

NS_CC_BEGIN

// implementation CCAtlasNode

// CCAtlasNode - Creation & Init

CCAtlasNode::CCAtlasNode()
: m_uItemsPerRow(0)
, m_uItemsPerColumn(0)
, m_uItemWidth(0)
, m_uItemHeight(0)
, m_bIsOpacityModifyRGB(false)
, m_uQuadsToDraw(0)
, m_bIgnoreContentScaleFactor(false)
{
}

CCAtlasNode::~CCAtlasNode()
{
}

CCAtlasNode * CCAtlasNode::create(const char *tile, unsigned int tileWidth, unsigned int tileHeight, 
											 unsigned int itemsToRender)
{
	CCAtlasNode * pRet = new CCAtlasNode();
	if (pRet->initWithTileFile(tile, tileWidth, tileHeight, itemsToRender))
	{
		pRet->autorelease();
		return pRet;
	}
	CC_SAFE_DELETE(pRet);
	return NULL;
}

bool CCAtlasNode::initWithTileFile(const char *tile, unsigned int tileWidth, unsigned int tileHeight, unsigned int itemsToRender)
{
    CCAssert(tile != NULL, "title should not be null");
	CCTexture2D *texture = SubSystem::get<ResourceCache>()->getResource<CCTexture2D>(tile);
	return initWithTexture(texture, tileWidth, tileHeight, itemsToRender);
}

bool CCAtlasNode::initWithTexture(CCTexture2D* texture, unsigned int tileWidth, unsigned int tileHeight, 
                                   unsigned int itemsToRender)
{
    m_uItemWidth  = tileWidth;
    m_uItemHeight = tileHeight;

    m_tColorUnmodified = ccWHITE;
    m_bIsOpacityModifyRGB = true;

    m_tBlendFunc.src = CC_BLEND_SRC;
    m_tBlendFunc.dst = CC_BLEND_DST;

    m_pTextureAtlas = new CCTextureAtlas();
    
    
    // shader stuff
    setRenderState(CCShaderCache::sharedShaderCache()->getRenderState(kCCShader_PositionTexture_uColor));
    ccGLUseProgram(_renderState->getGLProgram());
    m_pTextureAtlas->initWithTexture(texture, itemsToRender);

    if (! m_pTextureAtlas)
    {
        CCLOG("cocos2d: Could not initialize CCAtlasNode. Invalid Texture.");
        return false;
    }

    if (texture)
    {
        this->updateBlendFunc();
        this->updateOpacityModifyRGB();

        this->calculateMaxItems();
    }

    m_uQuadsToDraw = itemsToRender;

	_renderState->set4f("u_color", m_displayedColor.r / 255.0f, m_displayedColor.g / 255.0f, m_displayedColor.b / 255.0f, m_displayedOpacity / 255.0f);

    return true;
}


// CCAtlasNode - Atlas generation

void CCAtlasNode::calculateMaxItems()
{
    CCSize s = m_pTextureAtlas->getTexture()->getContentSize();
    
    if (m_bIgnoreContentScaleFactor)
    {
        s = m_pTextureAtlas->getTexture()->getContentSizeInPixels();
    }
    
    m_uItemsPerColumn = (int)(s.height / m_uItemHeight);
    m_uItemsPerRow = (int)(s.width / m_uItemWidth);
}

void CCAtlasNode::updateAtlasValues()
{
    CCAssert(false, "CCAtlasNode:Abstract updateAtlasValue not overridden");
}

// CCAtlasNode - draw
void CCAtlasNode::draw(void)
{
    CC_NODE_DRAW_SETUP();

    ccGLBlendFunc( m_tBlendFunc.src, m_tBlendFunc.dst );

    m_pTextureAtlas->drawNumberOfQuads(m_uQuadsToDraw, 0);
}

// CCAtlasNode - RGBA protocol

const ccColor3B& CCAtlasNode::getColor()
{
    if(m_bIsOpacityModifyRGB)
    {
        return m_tColorUnmodified;
    }
    return CCNode::getColor();
}

void CCAtlasNode::setColor(const ccColor3B& color3)
{
    ccColor3B tmp = color3;
    m_tColorUnmodified = color3;

    if( m_bIsOpacityModifyRGB )
    {
        tmp.r = tmp.r * m_displayedOpacity /255;
        tmp.g = tmp.g * m_displayedOpacity /255;
        tmp.b = tmp.b * m_displayedOpacity /255;
    }
	_renderState->set4f("u_color", m_displayedColor.r / 255.0f, m_displayedColor.g / 255.0f, m_displayedColor.b / 255.0f, m_displayedOpacity / 255.0f);
    CCNode::setColor(tmp);
}

void CCAtlasNode::setOpacity(GLubyte opacity)
{
    CCNode::setOpacity(opacity);

    // special opacity for premultiplied textures
    if( m_bIsOpacityModifyRGB )
        this->setColor(m_tColorUnmodified);
}

void CCAtlasNode::setOpacityModifyRGB(bool bValue)
{
    ccColor3B oldColor = this->getColor();
    m_bIsOpacityModifyRGB = bValue;
    this->setColor(oldColor);
}

bool CCAtlasNode::isOpacityModifyRGB()
{
    return m_bIsOpacityModifyRGB;
}

void CCAtlasNode::updateOpacityModifyRGB()
{
    m_bIsOpacityModifyRGB = m_pTextureAtlas->getTexture()->hasPremultipliedAlpha();
}

void CCAtlasNode::setIgnoreContentScaleFactor(bool bIgnoreContentScaleFactor)
{
    m_bIgnoreContentScaleFactor = bIgnoreContentScaleFactor;
}

// CCAtlasNode - CocosNodeTexture protocol

ccBlendFunc CCAtlasNode::getBlendFunc()
{
    return m_tBlendFunc;
}

void CCAtlasNode::setBlendFunc(ccBlendFunc blendFunc)
{
    m_tBlendFunc = blendFunc;
}

void CCAtlasNode::updateBlendFunc()
{
    if( ! m_pTextureAtlas->getTexture()->hasPremultipliedAlpha() ) {
        m_tBlendFunc.src = GL_SRC_ALPHA;
        m_tBlendFunc.dst = GL_ONE_MINUS_SRC_ALPHA;
    }
}

void CCAtlasNode::setTexture(CCTexture2D *texture)
{
    m_pTextureAtlas->setTexture(texture);
    this->updateBlendFunc();
    this->updateOpacityModifyRGB();
    this->calculateMaxItems();
}

CCTexture2D * CCAtlasNode::getTexture()
{
    return m_pTextureAtlas->getTexture();
}

unsigned int CCAtlasNode::getQuadsToDraw()
{
    return m_uQuadsToDraw;
}

void CCAtlasNode::setQuadsToDraw(unsigned int uQuadsToDraw)
{
    m_uQuadsToDraw = uQuadsToDraw;
}

NS_CC_END
