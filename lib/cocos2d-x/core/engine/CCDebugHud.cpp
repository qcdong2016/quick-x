#include "CCDebugHud.h"
#include "textures/CCTexture2D.h"
#include "ccFPSImages.h"
#include "CCImage.h"
#include "CCDirector.h"
#include "support/CCPointExtension.h"
#include "CCEngineEvents.h"

NS_CC_BEGIN

DebugHud::DebugHud()
	: _inteval(0)
	, _frames(0)
{
	createLabels();
}

void DebugHud::createLabels()
{

	CCTexture2DPixelFormat currentFormat = CCTexture2D::defaultAlphaPixelFormat();
	CCTexture2D::setDefaultAlphaPixelFormat(kCCTexture2DPixelFormat_RGBA4444);
	unsigned char *data = cc_fps_images_png;
	unsigned int data_len = cc_fps_images_len();

	SharedPtr<CCImage> image(new CCImage());
	bool isOK = image->initWithImageData(data, data_len);
	if (!isOK) {
		CCLOGERROR("%s", "Fails: init fps_images");
		return;
	}

	SharedPtr<CCTexture2D> texture(new CCTexture2D());
	texture->initWithImage(image);

	/*
	We want to use an image which is stored in the file named ccFPSImage.c
	for any design resolutions and all resource resolutions.

	To achieve this,

	Firstly, we need to ignore 'contentScaleFactor' in 'CCAtlasNode' and 'CCLabelAtlas'.
	So I added a new method called 'setIgnoreContentScaleFactor' for 'CCAtlasNode',
	this is not exposed to game developers, it's only used for displaying FPS now.

	Secondly, the size of this image is 480*320, to display the FPS label with correct size,
	a factor of design resolution ratio of 480x320 is also needed.
	*/
	float factor = 1.0f; // CCEGLView::sharedOpenGLView()->getDesignResolutionSize().height / 320.0f;

	_pFPSLabel = new CCLabelAtlas();
	_pFPSLabel->initWithString("00.0", texture, 12, 32, '.');
	_pFPSLabel->setScale(factor);

	_pSPFLabel = new CCLabelAtlas();
	_pSPFLabel->initWithString("0.000", texture, 12, 32, '.');
	_pSPFLabel->setScale(factor);

	_pDrawsLabel = new CCLabelAtlas();
	_pDrawsLabel->initWithString("000", texture, 12, 32, '.');
	_pDrawsLabel->setScale(factor);

	CCTexture2D::setDefaultAlphaPixelFormat(currentFormat);

#define CC_DIRECTOR_STATS_POSITION CCDirector::sharedDirector()->getVisibleOrigin()

	_pDrawsLabel->setPosition(ccpAdd(ccp(0, 34 * factor), CC_DIRECTOR_STATS_POSITION));
	_pSPFLabel->setPosition(ccpAdd(ccp(0, 17 * factor), CC_DIRECTOR_STATS_POSITION));
	_pFPSLabel->setPosition(CC_DIRECTOR_STATS_POSITION);

	subscribeToEvent<UpdateEvent>(Handler(this, &DebugHud::updateLabel));
	subscribeToEvent<AfterDraw>(Handler(this, &DebugHud::render));
}


void DebugHud::render(EventData& map)
{
	_pDrawsLabel->visit();
	_pFPSLabel->visit();
	_pSPFLabel->visit();
}

void DebugHud::updateLabel(EventData& map)
{
	float dt = map[UpdateEvent::timeStep].GetFloat();

	_inteval += dt;
	_frames++;

	if (_inteval > CC_DIRECTOR_STATS_INTERVAL)
	{
		static char buf[32] = { 0 };
		
		float frameRate = _frames / _inteval;

		_inteval = 0;
		_frames = 0;

#if COCOS2D_DEBUG > 0
		sprintf(buf, "%.1f %05d", frameRate, CCObject::s_livingCount);
#else
		sprintf(buf, "%.1f", frameRate);
#endif
		_pFPSLabel->setString(buf);

		sprintf(buf, "%4lu", (unsigned long)g_uNumberOfDraws);
		_pDrawsLabel->setString(buf);
	}
}

NS_CC_END

