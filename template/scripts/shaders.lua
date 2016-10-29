local function load(name)
    return assert(FileSystem:readAll(name), "Load `" .. name .. "` failed.")
end

local function build(v, f)
    assert(v, 'vert shader NULL')
    assert(f, 'frag shader NULL')
    return CCShaderCache:sharedShaderCache():addShader(v, f)
end

local function set1f(s, name, ...) s:set1f(s:get(name), ...) end
local function set2f(s, name, ...) s:set2f(s:get(name), ...) end
local function set3f(s, name, ...) s:set3f(s:get(name), ...) end
local function set4f(s, name, ...) s:set4f(s:get(name), ...) end
    
local gray_frag           = load("shaders/gray.frag")
local hblur_vert          = load("shaders/hblur.vert")
local vblur_vert          = load("shaders/vblur.vert")
local blur_frag           = load("shaders/blur.frag")
local sharpen_vert        = load("shaders/sharpen.vert")
local sharpen_frag        = load("shaders/sharpen.frag")
local rgb_frag            = load("shaders/rgb.frag")
local brightness_frag     = load("shaders/brightness.frag")
local exposure_frag       = load("shaders/exposure.frag")
local contrast_frag       = load("shaders/contrast.frag")
local saturation_frag     = load("shaders/saturation.frag")
local gamma_frag          = load("shaders/gamma.frag")
local hue_frag            = load("shaders/hue.frag")
local haze_frag           = load("shaders/haze.frag")
local zoom_blur_frag      = load("shaders/zoom_blur.frag")
local motion_blur_vert    = load("shaders/motion_blur.vert")
local motion_blur_frag    = load("shaders/motion_blur.frag")
local drop_shadow_vert    = load("shaders/drop_shadow.vert")
local drop_shadow_frag    = load("shaders/drop_shadow.frag")
local sepia_frag          = load("shaders/sepia.frag")
local test_vert           = load("shaders/test.vert")
local test_frag           = load("shaders/test.frag")
local gaussian_hblur_frag = load("shaders/gaussian_hblur.frag")
local gaussian_vblur_frag = load("shaders/gaussian_vblur.frag")
local colorramp_frag      = load("shaders/colorramp.frag")
local emboss_frag         = load("shaders/emboss.frag")
local embossmov_frag      = load("shaders/embossmov.frag")
local grass_frag          = load("shaders/grass.frag")

local ccPositionTextureColor_vert = load("shaders/ccPositionTextureColor.vert")
local ccPositionTexture_vert = load("shaders/ccPositionTexture.vert")

local SharpenFilter = build(sharpen_vert, sharpen_frag);
set1f(SharpenFilter, 'u_sharpness', 0)
set1f(SharpenFilter, 'u_widthFactor', 0)
set1f(SharpenFilter, 'u_heightFactor', 0)

local SepiaFilter = build(ccPositionTextureColor_vert, sepia_frag);
local SaturationFilter = build(ccPositionTexture_vert, saturation_frag);
set1f(SaturationFilter, 'u_saturation', 1)
local RGBFilter = build(ccPositionTexture_vert, rgb_frag)
set1f(RGBFilter, 'u_redAdj', 1)
set1f(RGBFilter, 'u_greenAdj', 1)
set1f(RGBFilter, 'u_blueAdj', 1)
-- CCMaskFilter
local HueFilter = build(ccPositionTexture_vert, hue_frag);
set1f(HueFilter, 'u_hueAdjust', 1)
local HazeFilter = build(ccPositionTexture_vert, haze_frag);
set1f(HazeFilter, 'u_hazeDistance', 0)
set1f(HazeFilter, 'u_slope', 0)
local GrayFilter = build(ccPositionTextureColor_vert, gray_frag);
set4f(GrayFilter, 'u_grayParam', 0.299, 0.587, 0.114, 0.0)
local GammaFilter = build(ccPositionTexture_vert, gamma_frag);
set1f(GammaFilter, 'u_gamma', 1)
local ExposureFilter = build(ccPositionTexture_vert, exposure_frag);
set1f(ExposureFilter, 'u_exposure', 0)
local DropShadowFilter = build(drop_shadow_vert, drop_shadow_frag);
set2f(DropShadowFilter, 'u_resolution', 480, 320)
local ContrastFilter = build(ccPositionTexture_vert, contrast_frag);
set1f(ContrastFilter, 'u_contrast', 1)
local BrightnessFilter = build(ccPositionTexture_vert, brightness_frag);
set1f(BrightnessFilter, 'u_brightness', 0)

local HBlurFilter = build(hblur_vert, blur_frag);
set1f(HBlurFilter, 'u_radius', 0.1)
    
local VBlurFilter = build(vblur_vert, blur_frag);
set1f(HBlurFilter, 'u_radius', 0.1)
    
local GaussianHBlurFilter = build(ccPositionTextureColor_vert, gaussian_hblur_frag);
set1f(GaussianHBlurFilter, 'u_resolution', 800)

local GaussianVBlurFilter = build(ccPositionTextureColor_vert, gaussian_vblur_frag);
set1f(GaussianVBlurFilter, 'u_resolution', 800)

local ZoomBlurFilter = build(ccPositionTexture_vert, zoom_blur_frag);
set1f(ZoomBlurFilter, 'u_blurSize', 1.0)
set2f(ZoomBlurFilter, 'u_blurCenter', 0.5, 0.5)

local MotionBlurFilter = build(motion_blur_vert, motion_blur_frag);
set2f(MotionBlurFilter, 'u_directionalTexelStep', 0.0, 0.0)


return  {
    Sharpen      = SharpenFilter,
    Sepia        = SepiaFilter,
    Saturation   = SaturationFilter,
    RGB          = RGBFilter,
    Hue          = HueFilter,
    Haze         = HazeFilter,
    Gray         = GrayFilter,
    Gamma        = GammaFilter,
    Exposure     = ExposureFilter,
    DropShadow   = DropShadowFilter,
    Contrast     = ContrastFilter,
    Brightness   = BrightnessFilter,
    HBlur        = HBlurFilter,
    VBlur        = VBlurFilter,
    GaussianHBlur= GaussianHBlurFilter,
    GaussianVBlur= GaussianVBlurFilter,
    ZoomBlur     = ZoomBlurFilter,
    MotionBlur   = MotionBlurFilter,
}


