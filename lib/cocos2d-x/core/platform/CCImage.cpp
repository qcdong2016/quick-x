/****************************************************************************
Copyright (c) 2010 cocos2d-x.org
Copyright (c) Microsoft Open Technologies, Inc.

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

#include "CCImage.h"
#include "CCStdC.h"
#include <string>
#include <ctype.h>
#include "engine/CCFileSystem.h"
#include "ccMacros.h"

#include "stb/stb_image.h"
#include "stb/stb_image_write.h"

NS_CC_BEGIN

// premultiply alpha, or the effect will wrong when want to use other pixel format in CCTexture2D,
// such as RGB888, RGB5A1
#define CC_RGB_PREMULTIPLY_ALPHA(vr, vg, vb, va) \
    (unsigned)(((unsigned)((unsigned char)(vr) * ((unsigned char)(va) + 1)) >> 8) | \
    ((unsigned)((unsigned char)(vg) * ((unsigned char)(va) + 1) >> 8) << 8) | \
    ((unsigned)((unsigned char)(vb) * ((unsigned char)(va) + 1) >> 8) << 16) | \
    ((unsigned)(unsigned char)(va) << 24))


//////////////////////////////////////////////////////////////////////////
// Implement CCImage
//////////////////////////////////////////////////////////////////////////

CCImage::CCImage()
: m_nWidth(0)
, m_nHeight(0)
, m_nBitsPerComponent(0)
, m_pData(0)
, m_bHasAlpha(false)
, m_bPreMulti(false)
{
}

CCImage::~CCImage()
{
    CC_SAFE_DELETE_ARRAY(m_pData);
}

bool CCImage::initWithImageFile(const char * strPath)
{
	SharedPtr<MemBuffer> bf = FileSystem::readAll(strPath);

	if (!bf->isNull())
		return initWithImageData(bf->getData(), bf->getSize());

    return false;
}

bool CCImage::initWithImageData(void * pData, int nDataLen)
{
    int w, h, comp;
    unsigned char* decoded = stbi_load_from_memory((stbi_uc const *)pData, nDataLen, &w, &h, &comp, 0);
    
    if (!decoded)
        return false;
    
    m_nWidth = w;
    m_nHeight = h;
    
    m_nBitsPerComponent = comp * 8;
    if (comp == 4)
        m_bHasAlpha = true;
    m_bPreMulti = false;

    size_t size = w * h * m_nBitsPerComponent;
    m_pData = new unsigned char[size];
    memcpy(m_pData, decoded, size);
    stbi_image_free(decoded);

    return true;
}


bool CCImage::initWithRawData(void *pData, int nDatalen, int nWidth, int nHeight, int nBitsPerComponent, bool bPreMulti)
{
    bool bRet = false;
    do
    {
        CC_BREAK_IF(0 == nWidth || 0 == nHeight);
        
        m_nBitsPerComponent = nBitsPerComponent;
        m_nHeight   = (short)nHeight;
        m_nWidth    = (short)nWidth;
        m_bHasAlpha = true;
        
        // only RGBA8888 supported
        int nBytesPerComponent = 4;
        int nSize = nHeight * nWidth * nBytesPerComponent;
        m_pData = new unsigned char[nSize];
        CC_BREAK_IF(! m_pData);
        memcpy(m_pData, pData, nSize);
        
        bRet = true;
    } while (0);
    return bRet;
}


bool CCImage::saveToFile(const char* filename)
{
    std::string name = filename;
    
    if (!FileSystem::isAbsolutePath(filename))
        name = FileSystem::join(FileSystem::getWritablePath(), filename);
    
    std::string ext = FileSystem::getExtension(name);
    
    int comp = m_nBitsPerComponent / 8;
    if (ext == "bmp") {
        stbi_write_bmp(name.c_str(), m_nWidth, m_nHeight, comp, m_pData);
    } else if (ext == "png") {
        stbi_write_png(name.c_str(), m_nWidth, m_nHeight, comp, m_pData, 0);
    } else if (ext == "tga") {
        stbi_write_tga(name.c_str(), m_nWidth, m_nHeight, comp, m_pData);
    } else {
        return false;
    }
    
    return true;
}

/*

#if CC_WEBP_ENABLED > 0
#include "webp/decode.h"

bool CCImage::_initWithWebpData(void *pData, int nDataLen)
{
    bool bRet = false;
    do
    {
        WebPDecoderConfig config;
        if (WebPInitDecoderConfig(&config) == 0) break;
        if (WebPGetFeatures((uint8_t*)pData, nDataLen, &config.input) != VP8_STATUS_OK) break;
        if (config.input.width == 0 || config.input.height == 0) break;
        
        config.output.colorspace = MODE_RGBA;
        m_nBitsPerComponent = 8;
        m_nWidth = config.input.width;
        m_nHeight = config.input.height;
        m_bHasAlpha = true;
        
        int bufferSize = m_nWidth * m_nHeight * 4;
        m_pData = new unsigned char[bufferSize];
        
        config.output.u.RGBA.rgba = (uint8_t*)m_pData;
        config.output.u.RGBA.stride = m_nWidth * 4;
        config.output.u.RGBA.size = bufferSize;
        config.output.is_external_memory = 1;
        
        if (WebPDecode((uint8_t*)pData, nDataLen, &config) != VP8_STATUS_OK)
        {
            delete[]m_pData;
            m_pData = NULL;
            break;
        }
        
        bRet = true;
    } while (0);
    
    return bRet;
}
#endif // CC_WEBP_ENABLED

 */

NS_CC_END

