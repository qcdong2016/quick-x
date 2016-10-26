/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (c) 2011      Ricardo Quesada
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

#include "CCShaderCache.h"
#include "CCGLProgram.h"
#include "ccMacros.h"static 

static const GLchar * ccPosition_uColor_frag =
#include "ccShader_Position_uColor_frag.h"
static const GLchar * ccPosition_uColor_vert =
#include "ccShader_Position_uColor_vert.h"

//
static const GLchar * ccPositionColor_frag =
#include "ccShader_PositionColor_frag.h"
static const GLchar * ccPositionColor_vert =
#include "ccShader_PositionColor_vert.h"

//
static const GLchar * ccPositionTexture_frag =
#include "ccShader_PositionTexture_frag.h"
static const GLchar * ccPositionTexture_vert =
#include "ccShader_PositionTexture_vert.h"

//
static const GLchar * ccPositionTextureA8Color_frag =
#include "ccShader_PositionTextureA8Color_frag.h"
static const GLchar * ccPositionTextureA8Color_vert =
#include "ccShader_PositionTextureA8Color_vert.h"

//
static const GLchar * ccPositionTextureColor_frag =
#include "ccShader_PositionTextureColor_frag.h"
static const GLchar * ccPositionTextureColor_vert =
#include "ccShader_PositionTextureColor_vert.h"
static const GLchar * ccPositionTextureGray_frag =
#include "ccShader_PositionTextureGray_frag.h"

//
static const GLchar * ccPositionTextureColorAlphaTest_frag =
#include "ccShader_PositionTextureColorAlphaTest_frag.h"

//
static const GLchar * ccPositionTexture_uColor_frag =
#include "ccShader_PositionTexture_uColor_frag.h"
static const GLchar * ccPositionTexture_uColor_vert =
#include "ccShader_PositionTexture_uColor_vert.h"

static const GLchar * ccPositionColorLengthTexture_frag =
#include "ccShader_PositionColorLengthTexture_frag.h"
static const GLchar * ccPositionColorLengthTexture_vert =
#include "ccShader_PositionColorLengthTexture_vert.h"


NS_CC_BEGIN

static CCShaderCache *_sharedShaderCache = 0;

CCShaderCache* CCShaderCache::sharedShaderCache()
{
    if (!_sharedShaderCache) {
        _sharedShaderCache = new CCShaderCache();
        if (!_sharedShaderCache->init())
        {
            CC_SAFE_DELETE(_sharedShaderCache);
        }
    }
    return _sharedShaderCache;
}

void CCShaderCache::purgeSharedShaderCache()
{
    CC_SAFE_RELEASE_NULL(_sharedShaderCache);
}

CCShaderCache::CCShaderCache()
{

}

CCShaderCache::~CCShaderCache()
{
}



bool CCShaderCache::init()
{
    loadDefaultShaders();
    return true;
}

#define loadShader(type) \
	SharedPtr<CCGLProgram> type(new CCGLProgram()); \
    loadDefaultShader(type, kCCShader_##type); \
	_defaultShaders[kCCShader_##type] = type;


void CCShaderCache::loadDefaultShaders()
{
    loadShader(PositionTextureColor);
    loadShader(PositionTextureGray);
    loadShader(PositionTextureColorAlphaTest);
    loadShader(PositionColor);
    loadShader(PositionTexture);
    loadShader(PositionTexture_uColor);
    loadShader(PositionTextureA8Color);
    loadShader(Position_uColor);
    loadShader(PositionLengthTexureColor);
}

void CCShaderCache::reloadDefaultShaders()
{
	_defaultShaders.clear();
	loadDefaultShaders();
}


void CCShaderCache::loadDefaultShader(CCGLProgram *p, int type)
{
	switch (type) {
	case kCCShader_PositionTextureColor:
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColor_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_PositionTextureGray:
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureGray_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_PositionTextureColorAlphaTest:
		p->initWithVertexShaderByteArray(ccPositionTextureColor_vert, ccPositionTextureColorAlphaTest_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_PositionColor:
		p->initWithVertexShaderByteArray(ccPositionColor_vert, ccPositionColor_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);

		break;
	case kCCShader_PositionTexture:
		p->initWithVertexShaderByteArray(ccPositionTexture_vert, ccPositionTexture_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_PositionTexture_uColor:
		p->initWithVertexShaderByteArray(ccPositionTexture_uColor_vert, ccPositionTexture_uColor_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_PositionTextureA8Color:
		p->initWithVertexShaderByteArray(ccPositionTextureA8Color_vert, ccPositionTextureA8Color_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);

		break;
	case kCCShader_Position_uColor:
		p->initWithVertexShaderByteArray(ccPosition_uColor_vert, ccPosition_uColor_frag);

		p->addAttribute("aVertex", kCCVertexAttrib_Position);

		break;
	case kCCShader_PositionLengthTexureColor:
		p->initWithVertexShaderByteArray(ccPositionColorLengthTexture_vert, ccPositionColorLengthTexture_frag);

		p->addAttribute(kCCAttributeNamePosition, kCCVertexAttrib_Position);
		p->addAttribute(kCCAttributeNameTexCoord, kCCVertexAttrib_TexCoords);
		p->addAttribute(kCCAttributeNameColor, kCCVertexAttrib_Color);

		break;

	default:
		CCLOG("cocos2d: %s:%d, error shader type", __FUNCTION__, __LINE__);
		return;
	}
    
    p->link();
    p->updateUniforms();
    
    CHECK_GL_ERROR_DEBUG();
}

CCGLProgram* CCShaderCache::programForKey(int key)
{
	auto it = _defaultShaders.find(key);
	if (it != _defaultShaders.end())
		return it->second.Get();
	return nullptr;
}

void CCShaderCache::addProgram(CCGLProgram* program, int key)
{
	_defaultShaders[key] = program;
}

NS_CC_END
