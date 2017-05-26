
require("config")
require("framework.init")

local App = class("App")

function App:ctor()
    app = self
end


function App:enterState(name, ...)
    if self.state_ then
        self.state_:removeSelf()
    end

    print('enterState', name)

    self.statename = name
    self.state_ = require('states.'..name).new(...):addTo(self.canvas)
    self.state_:setNodeEventEnabled(true)
end

function App:run()
    self.scene_ = director:getScene()

    self.canvas = display.newNode():addTo(self.scene_)
        
    UIMgr = require('UIMgr').new():addTo(self.canvas, 2)

    FileSystem:addResourcePath("res/")

    self:screenFit(960, 640, "FIXED_HEIGHT")
    self:enterState("Menu")
end

function App:onEnterBackground()
end

function App:onEnterForeground()
end

function App:exit()
    CCDirector:sharedDirector():endToLua()
    if device.platform ~= "ios" then
        os.exit()
    end
end

function App:screenFit(CONFIG_SCREEN_WIDTH, CONFIG_SCREEN_HEIGHT, CONFIG_SCREEN_AUTOSCALE)

    local sharedDirector = director
    local resourceCache  = sharedDirector.resourceCache

    -- check device screen size
    local glview = sharedDirector:getOpenGLView()
    local size = glview:getFrameSize()
    display.sizeInPixels = {width = size.width, height = size.height}

    local w = display.sizeInPixels.width
    local h = display.sizeInPixels.height

    if CONFIG_SCREEN_WIDTH == nil or CONFIG_SCREEN_HEIGHT == nil then
        CONFIG_SCREEN_WIDTH = w
        CONFIG_SCREEN_HEIGHT = h
    end

    local scale = 1

    if not CONFIG_SCREEN_AUTOSCALE then
        if w > h then
            CONFIG_SCREEN_AUTOSCALE = "FIXED_HEIGHT"
        else
            CONFIG_SCREEN_AUTOSCALE = "FIXED_WIDTH"
        end
    else
        CONFIG_SCREEN_AUTOSCALE = string.upper(CONFIG_SCREEN_AUTOSCALE)
    end

    local scaleX, scaleY

    if CONFIG_SCREEN_AUTOSCALE then
        if type(CONFIG_SCREEN_AUTOSCALE_CALLBACK) == "function" then
            scaleX, scaleY = CONFIG_SCREEN_AUTOSCALE_CALLBACK(w, h, device.model)
        end

        if not scaleX or not scaleY then
            scaleX, scaleY = w / CONFIG_SCREEN_WIDTH, h / CONFIG_SCREEN_HEIGHT
        end

        if CONFIG_SCREEN_AUTOSCALE == "FIXED_WIDTH" then
            scale = scaleX
            CONFIG_SCREEN_HEIGHT = h / scale
        elseif CONFIG_SCREEN_AUTOSCALE == "FIXED_HEIGHT" then
            scale = scaleY
            CONFIG_SCREEN_WIDTH = w / scale
        else
            scale = 1.0
            printError(string.format("display - invalid CONFIG_SCREEN_AUTOSCALE \"%s\"", CONFIG_SCREEN_AUTOSCALE))
        end

        self.canvas:setScale(scale)
    end

    local winSize = sharedDirector:getWinSize()
    display.contentScaleFactor = scale
    display.size               = {width = winSize.width, height = winSize.height}
    display.width              = display.size.width
    display.height             = display.size.height
    display.cx                 = display.width / 2
    display.cy                 = display.height / 2
    display.c_left             = -display.width / 2
    display.c_right            = display.width / 2
    display.c_top              = display.height / 2
    display.c_bottom           = -display.height / 2
    display.left               = 0
    display.right              = display.width
    display.top                = display.height
    display.bottom             = 0
    display.widthInPixels      = display.sizeInPixels.width
    display.heightInPixels     = display.sizeInPixels.height

    printInfo(string.format("# CONFIG_SCREEN_AUTOSCALE      = %s", CONFIG_SCREEN_AUTOSCALE))
    printInfo(string.format("# CONFIG_SCREEN_WIDTH          = %0.2f", CONFIG_SCREEN_WIDTH))
    printInfo(string.format("# CONFIG_SCREEN_HEIGHT         = %0.2f", CONFIG_SCREEN_HEIGHT))
    printInfo(string.format("# display.widthInPixels        = %0.2f", display.widthInPixels))
    printInfo(string.format("# display.heightInPixels       = %0.2f", display.heightInPixels))
    printInfo(string.format("# display.contentScaleFactor   = %0.2f", display.contentScaleFactor))
    printInfo(string.format("# display.width                = %0.2f", display.width))
    printInfo(string.format("# display.height               = %0.2f", display.height))
    printInfo(string.format("# display.cx                   = %0.2f", display.cx))
    printInfo(string.format("# display.cy                   = %0.2f", display.cy))
    printInfo(string.format("# display.left                 = %0.2f", display.left))
    printInfo(string.format("# display.right                = %0.2f", display.right))
    printInfo(string.format("# display.top                  = %0.2f", display.top))
    printInfo(string.format("# display.bottom               = %0.2f", display.bottom))
    printInfo(string.format("# display.c_left               = %0.2f", display.c_left))
    printInfo(string.format("# display.c_right              = %0.2f", display.c_right))
    printInfo(string.format("# display.c_top                = %0.2f", display.c_top))
    printInfo(string.format("# display.c_bottom             = %0.2f", display.c_bottom))
    printInfo("#")
end

return App
