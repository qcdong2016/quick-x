/****************************************************************************
Copyright (c) 2010-2012 cocos2d-x.org
Copyright (C) 2008      Apple Inc. All Rights Reserved.

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

#ifndef __CCTEXTURE2D_H__
#define __CCTEXTURE2D_H__

#include "engine/CCResourceCache.h"
#include "engine/CCObject.h"
#include "cocoa/CCGeometry.h"
#include "ccTypes.h"
#include <string>

NS_CC_BEGIN

class CCImage;
class Material;
/**
 * @addtogroup textures
 * @{
 */

//CONSTANTS:

/** @typedef CCTexture2DPixelFormat
Possible texture pixel formats
*/
typedef enum {

    //! 32-bit texture: RGBA8888
    kCCTexture2DPixelFormat_RGBA8888,
    //! 24-bit texture: RGBA888
    kCCTexture2DPixelFormat_RGB888,
    //! 16-bit texture without Alpha channel
    kCCTexture2DPixelFormat_RGB565,
    //! 8-bit textures used as masks
    kCCTexture2DPixelFormat_A8,
    //! 8-bit intensity texture
    kCCTexture2DPixelFormat_I8,
    //! 16-bit textures used as masks
    kCCTexture2DPixelFormat_AI88,
    //! 16-bit textures: RGBA4444
    kCCTexture2DPixelFormat_RGBA4444,
    //! 16-bit textures: RGB5A1
    kCCTexture2DPixelFormat_RGB5A1,    
    //! 4-bit PVRTC-compressed texture: PVRTC4
    kCCTexture2DPixelFormat_PVRTC4,
    //! 2-bit PVRTC-compressed texture: PVRTC2
    kCCTexture2DPixelFormat_PVRTC2,


    //! Default texture format: RGBA8888
    kCCTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_RGBA8888,

    // backward compatibility stuff
    kTexture2DPixelFormat_RGBA8888 = kCCTexture2DPixelFormat_RGBA8888,
    kTexture2DPixelFormat_RGB888 = kCCTexture2DPixelFormat_RGB888,
    kTexture2DPixelFormat_RGB565 = kCCTexture2DPixelFormat_RGB565,
    kTexture2DPixelFormat_A8 = kCCTexture2DPixelFormat_A8,
    kTexture2DPixelFormat_RGBA4444 = kCCTexture2DPixelFormat_RGBA4444,
    kTexture2DPixelFormat_RGB5A1 = kCCTexture2DPixelFormat_RGB5A1,
    kTexture2DPixelFormat_Default = kCCTexture2DPixelFormat_Default

} CCTexture2DPixelFormat;

class CCGLProgram;

/**
Extension to set the Min / Mag filter
*/
typedef struct _ccTexParams {
    GLuint    minFilter;
    GLuint    magFilter;
    GLuint    wrapS;
    GLuint    wrapT;
} ccTexParams;

//CLASS INTERFACES:

/** @brief CCTexture2D class.
* This class allows to easily create OpenGL 2D textures from images, text or raw data.
* The created CCTexture2D object will always have power-of-two dimensions. 
* Depending on how you create the CCTexture2D object, the actual image area of the texture might be smaller than the texture dimensions i.e. "contentSize" != (pixelsWide, pixelsHigh) and (maxS, maxT) != (1.0, 1.0).
* Be aware that the content of the generated textures will be upside-down!
*/
class CC_DLL CCTexture2D : public Resource
{
	CCOBJECT(CCTexture2D, Resource)

public:
    /**
     * @js ctor
     */
    CCTexture2D();
    /**
     * @js NA
     * @lua NA
     */
    virtual ~CCTexture2D();
	virtual void beginLoad(MemBuffer* buf, void* userdata);

    /**
     *  @js NA
     *  @lua NA
     */
    const char* description(void);

    /** These functions are needed to create mutable textures 
     * @js NA
     */
    void releaseData(void *data);
    /**
     * @js NA
     */
    void* keepData(void *data, unsigned int length);

	bool initWithFile(const char* filename);
    /** Initializes with a texture2d with data 
     * @js NA
     * @lua NA
     */
    bool initWithData(const void* data, CCTexture2DPixelFormat pixelFormat, unsigned int pixelsWide, unsigned int pixelsHigh, const CCSize& contentSize);

    /**
    Drawing extensions to make it easy to draw basic quads using a CCTexture2D object.
    These functions require GL_TEXTURE_2D and both GL_VERTEX_ARRAY and GL_TEXTURE_COORD_ARRAY client states to be enabled.
    */
    /** draws a texture at a given point */
    void drawAtPoint(const CCPoint& point);
    /** draws a texture inside a rect */
    void drawInRect(const CCRect& rect);

    /**
    Extensions to make it easy to create a CCTexture2D object from an image file.
    Note that RGBA type textures will have their alpha premultiplied - use the blending mode (GL_ONE, GL_ONE_MINUS_SRC_ALPHA).
    */
    /** Initializes a texture from a UIImage object */

    bool initWithImage(CCImage * uiImage);

    /** Initializes a texture from a string with dimensions, alignment, font name and font size */
    bool initWithString(const char *text,  const char *fontName, float fontSize, const CCSize& dimensions, CCTextAlignment hAlignment, CCVerticalTextAlignment vAlignment);
    /** Initializes a texture from a string with font name and font size */
    bool initWithString(const char *text, const char *fontName, float fontSize);
    /** Initializes a texture from a string using a text definition*/
    bool initWithString(const char *text, ccFontDefinition *textDefinition);
    
    /** Initializes a texture from a PVR file */
    bool initWithPVRFile(const char* file);
    
    /** Initializes a texture from a ETC file */
    bool initWithETCFile(const char* file);

    /** sets the min filter, mag filter, wrap s and wrap t texture parameters.
    If the texture size is NPOT (non power of 2), then in can only use GL_CLAMP_TO_EDGE in GL_TEXTURE_WRAP_{S,T}.

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    @code
    when this functon bound to js,the input param are changed
    js: var setTexParameters(var minFilter, var magFilter, var wrapS, var wrapT)
    @endcode
    */
    void setTexParameters(ccTexParams* texParams);

    /** sets antialias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_LINEAR
    - GL_TEXTURE_MAG_FILTER = GL_LINEAR

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */
    void setAntiAliasTexParameters();

    /** sets alias texture parameters:
    - GL_TEXTURE_MIN_FILTER = GL_NEAREST
    - GL_TEXTURE_MAG_FILTER = GL_NEAREST

    @warning Calling this method could allocate additional texture memory.

    @since v0.8
    */
    void setAliasTexParameters();


    /** Generates mipmap images for the texture.
    It only works if the texture size is POT (power of 2).
    @since v0.99.0
    */
    void generateMipmap();

    /** returns the pixel format.
     @since v2.0
     */
    const char* stringForFormat();

    /** returns the bits-per-pixel of the in-memory OpenGL texture
    @since v1.0
    */
    unsigned int bitsPerPixelForFormat();  

    /** Helper functions that returns bits per pixels for a given format.
     @since v2.0
     */
    unsigned int bitsPerPixelForFormat(CCTexture2DPixelFormat format);

    /** sets the default pixel format for UIImagescontains alpha channel.
    If the UIImage contains alpha channel, then the options are:
    - generate 32-bit textures: kCCTexture2DPixelFormat_RGBA8888 (default one)
    - generate 24-bit textures: kCCTexture2DPixelFormat_RGB888
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGBA4444
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB5A1
    - generate 16-bit textures: kCCTexture2DPixelFormat_RGB565
    - generate 8-bit textures: kCCTexture2DPixelFormat_A8 (only use it if you use just 1 color)

    How does it work ?
    - If the image is an RGBA (with Alpha) then the default pixel format will be used (it can be a 8-bit, 16-bit or 32-bit texture)
    - If the image is an RGB (without Alpha) then: If the default pixel format is RGBA8888 then a RGBA8888 (32-bit) will be used. Otherwise a RGB565 (16-bit texture) will be used.

    This parameter is not valid for PVR / PVR.CCZ images.

    @since v0.8
    */
    static void setDefaultAlphaPixelFormat(CCTexture2DPixelFormat format);

    /** returns the alpha pixel format
    @since v0.8
    @js getDefaultAlphaPixelFormat
    */
    static CCTexture2DPixelFormat defaultAlphaPixelFormat();

    /** treats (or not) PVR files as if they have alpha premultiplied.
     Since it is impossible to know at runtime if the PVR images have the alpha channel premultiplied, it is
     possible load them as if they have (or not) the alpha channel premultiplied.
     
     By default it is disabled.
     
     @since v0.99.5
     */
    static void PVRImagesHavePremultipliedAlpha(bool haveAlphaPremultiplied);

    /** content size */
    const CCSize& getContentSizeInPixels();
    
    bool hasPremultipliedAlpha();
    bool hasMipmaps();
private:
    bool initPremultipliedATextureWithImage(CCImage * image, unsigned int pixelsWide, unsigned int pixelsHigh);
    
    // By default PVR images are treated as if they don't have the alpha channel premultiplied
    bool m_bPVRHaveAlphaPremultiplied;

    /** pixel format of the texture */
    CC_PROPERTY_READONLY(CCTexture2DPixelFormat, m_ePixelFormat, PixelFormat)
    /** width in pixels */
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsWide, PixelsWide)
    /** height in pixels */
    CC_PROPERTY_READONLY(unsigned int, m_uPixelsHigh, PixelsHigh)

    /** texture name */
    CC_PROPERTY_READONLY(GLuint, m_uName, Name)

    /** texture max S */
    CC_PROPERTY(GLfloat, m_fMaxS, MaxS)
    /** texture max T */
    CC_PROPERTY(GLfloat, m_fMaxT, MaxT)
    /** content size */
    CC_PROPERTY_READONLY(CCSize, m_tContentSize, ContentSize)

    /** whether or not the texture has their Alpha premultiplied */
    bool m_bHasPremultipliedAlpha;

    bool m_bHasMipmaps;

	Material* _material;
};


#if CC_ENABLE_CACHE_TEXTURE_DATA

class VolatileTexture
{
	typedef enum {
		kInvalid = 0,
		kImageFile,
		kImageData,
		kString,
		kImage,
	}ccCachedImageType;

public:
	VolatileTexture(CCTexture2D *t);
	~VolatileTexture();

	static void addImageTexture(CCTexture2D *tt, const char* imageFileName, EImageFormat format);
	static void addStringTexture(CCTexture2D *tt, const char* text, const CCSize& dimensions, CCTextAlignment alignment,
		CCVerticalTextAlignment vAlignment, const char *fontName, float fontSize);
	static void addDataTexture(CCTexture2D *tt, void* data, CCTexture2DPixelFormat pixelFormat, const CCSize& contentSize);
	static void addCCImage(CCTexture2D *tt, CCImage *image);

	static void setTexParameters(CCTexture2D *t, ccTexParams *texParams);
	static void removeTexture(CCTexture2D *t);
	static void reloadAllTextures();

public:
	static std::list<VolatileTexture*> textures;
	static bool isReloading;

private:
	// find VolatileTexture by CCTexture2D*
	// if not found, create a new one
	static VolatileTexture* findVolotileTexture(CCTexture2D *tt);

protected:
	CCTexture2D *texture;

	CCImage *uiImage;

	ccCachedImageType m_eCashedImageType;

	void *m_pTextureData;
	CCSize m_TextureSize;
	CCTexture2DPixelFormat m_PixelFormat;

	std::string m_strFileName;
	EImageFormat m_FmtImage;

	ccTexParams     m_texParams;
	CCSize          m_size;
	CCTextAlignment m_alignment;
	CCVerticalTextAlignment m_vAlignment;
	std::string     m_strFontName;
	std::string     m_strText;
	float           m_fFontSize;
};

#endif

// end of textures group
/// @}

NS_CC_END

#endif //__CCTEXTURE2D_H__

