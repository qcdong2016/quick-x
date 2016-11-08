local function load(name)
    return assert(FileSystem:readAll(name), "Load `" .. name .. "` failed.")
end

local function build(v, f)
    assert(v, 'vert shader NULL')
    assert(f, 'frag shader NULL')
    return CCShaderCache:sharedShaderCache():addMaterialFromShaderSource(v, f)
end
    
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

if imgui then
    require "imgui_ctrl"
end
local ctrls = {
    float_ctrl,
    float2_ctrl,
    float3_ctrl,
    float4_ctrl,
}

local function make(v, f, args)
    local filter = build(v, f);
    local editors = {}
    for name, value_table in pairs(args) do
        local ctrl = ctrls[#value_table]
        local set = filter['set'..#value_table..'f']

        if not filter:getParam(name) then
            print('error asdfkajdf ', name)
        end

        set(filter, name, unpack(value_table))
        if imgui then
            table.insert(editors, ctrl(name, function()
                return unpack(value_table)
            end, function(...)
                value_table = {...}
                set(filter, name, ...)
            end))
        end
    end

    filter.editors = editors

    return filter
end

local SharpenFilter = make(sharpen_vert, sharpen_frag, {
    ['u_sharpness'] = { 0,},
    ['u_widthFactor'] = { 0,},
    ['u_heightFactor'] = { 0,},
})

local SepiaFilter = make(ccPositionTextureColor_vert, sepia_frag, {})
local SaturationFilter = make(ccPositionTexture_vert, saturation_frag, {
    ['u_saturation'] = { 1,},
});
local RGBFilter = make(ccPositionTexture_vert, rgb_frag, {
    ['u_redAdj'] = { 1 },
    ['u_greenAdj'] = { 1 },
    ['u_blueAdj'] = { 1 },
});
local HueFilter = make(ccPositionTexture_vert, hue_frag, {
    ['u_hueAdjust'] = { 1 },
})
local HazeFilter = make(ccPositionTexture_vert, haze_frag, {
    ['u_hazeDistance'] = { 0 },
    ['u_slope'] = { 0 },
})
local GrayFilter = make(ccPositionTextureColor_vert, gray_frag, {
    ['u_grayParam'] = { 0.299, 0.587, 0.114, 0.0 },
})
local GammaFilter = make(ccPositionTexture_vert, gamma_frag, {
    ['u_gamma'] = { 1 },
})
local ExposureFilter = make(ccPositionTexture_vert, exposure_frag, {
    ['u_exposure'] = { 0 },
})
local DropShadowFilter = make(drop_shadow_vert, drop_shadow_frag, {
    ['u_resolution'] = { 480, 320 },
})
local ContrastFilter = make(ccPositionTexture_vert, contrast_frag, {
    ['u_contrast'] = { 1 },
})
local BrightnessFilter = make(ccPositionTexture_vert, brightness_frag, {
    ['u_brightness'] = { 0 },
})

local HBlurFilter = make(hblur_vert, blur_frag, {
    ['u_radius'] = { 0.1 },
})
    
local VBlurFilter = make(vblur_vert, blur_frag, {
    ['u_radius'] = { 0.1 },
})
    
local GaussianHBlurFilter = make(ccPositionTextureColor_vert, gaussian_hblur_frag, {
    ['u_resolution'] = { 800 },
    ['u_radius'] = { 0.1 },
})

local GaussianVBlurFilter = make(ccPositionTextureColor_vert, gaussian_vblur_frag, {
    ['u_resolution'] = { 800 },
    ['u_radius'] = { 0.1 },
})

local ZoomBlurFilter = make(ccPositionTexture_vert, zoom_blur_frag, {
    ['u_blurSize'] = { 1.0 },
    ['u_blurCenter'] = { 0.5, 0.5 },
})

local MotionBlurFilter = make(motion_blur_vert, motion_blur_frag, {
    ['u_directionalTexelStep'] = { 0.0, 0.0 },
})

local Wave = make(ccPositionTextureColor_vert, load('shaders/wave.frag'), {
    ['u_time'] = { 0.0, },
    ['u_resolution'] = { 500.0, 500.0},
})
-- local SharpenFilter = build(sharpen_vert, sharpen_frag);
-- SharpenFilter:set1f('u_sharpness', 0)
-- SharpenFilter:set1f('u_widthFactor', 0)
-- SharpenFilter:set1f('u_heightFactor', 0)

-- local SepiaFilter = build(ccPositionTextureColor_vert, sepia_frag);
-- local SaturationFilter = build(ccPositionTexture_vert, saturation_frag);
-- SaturationFilter:set1f('u_saturation', 1)
-- local RGBFilter = build(ccPositionTexture_vert, rgb_frag)
-- RGBFilter:set1f('u_redAdj', 1)
-- RGBFilter:set1f('u_greenAdj', 1)
-- RGBFilter:set1f('u_blueAdj', 1)
-- CCMaskFilter
-- local HueFilter = build(ccPositionTexture_vert, hue_frag);
-- HueFilter:set1f('u_hueAdjust', 1)
-- local HazeFilter = build(ccPositionTexture_vert, haze_frag);
-- HazeFilter:set1f('u_hazeDistance', 0)
-- HazeFilter:set1f('u_slope', 0)
-- local GrayFilter = build(ccPositionTextureColor_vert, gray_frag);
-- GrayFilter:set4f('u_grayParam', 0.299, 0.587, 0.114, 0.0)
-- local GammaFilter = build(ccPositionTexture_vert, gamma_frag);
-- GammaFilter:set1f('u_gamma', 1)
-- local ExposureFilter = build(ccPositionTexture_vert, exposure_frag);
-- ExposureFilter:set1f('u_exposure', 0)
-- local DropShadowFilter = build(drop_shadow_vert, drop_shadow_frag);
-- DropShadowFilter:set2f('u_resolution', 480, 320)
-- local ContrastFilter = build(ccPositionTexture_vert, contrast_frag);
-- ContrastFilter:set1f('u_contrast', 1)
-- local BrightnessFilter = build(ccPositionTexture_vert, brightness_frag);
-- BrightnessFilter:set1f('u_brightness', 0)

-- local HBlurFilter = build(hblur_vert, blur_frag);
-- HBlurFilter:set1f('u_radius', 0.1)
    
-- local VBlurFilter = build(vblur_vert, blur_frag);
-- VBlurFilter:set1f('u_radius', 0.1)
    
-- local GaussianHBlurFilter = build(ccPositionTextureColor_vert, gaussian_hblur_frag);
-- GaussianHBlurFilter:set1f('u_resolution', 800)

-- local GaussianVBlurFilter = build(ccPositionTextureColor_vert, gaussian_vblur_frag);
-- GaussianVBlurFilter:set1f('u_resolution', 800)

-- local ZoomBlurFilter = build(ccPositionTexture_vert, zoom_blur_frag);
-- ZoomBlurFilter:set1f('u_blurSize', 1.0)
-- ZoomBlurFilter:set2f('u_blurCenter', 0.5, 0.5)

-- local MotionBlurFilter = build(motion_blur_vert, motion_blur_frag);
-- MotionBlurFilter:set2f('u_directionalTexelStep', 0.0, 0.0)




return  {
    Wave         = Wave,
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


