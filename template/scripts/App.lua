
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
    self.state_ = require('states.'..name).new(...):addTo(self.scene_)
    self.state_:setNodeEventEnabled(true)
end

function App:run()
    self.scene_ = display.newScene("Scene")
    -- self.setting = cc.utils.setting.new('local.lua', 'MyKey')
    
    UIMgr = require('UIMgr').new():addTo(self.scene_, 2)

    display.replaceScene(self.scene_)

    FileSystem:addResourcePath("res/")

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

return App
