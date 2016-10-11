
require("config")
require("framework.init")

local App = class("App", cc.mvc.AppBase)

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
    self.setting = cc.utils.setting.new('local.lua', 'MyKey')
    
    UIMgr = require('UIMgr').new():addTo(self.scene_, 2)

    display.replaceScene(self.scene_)

    CCFileUtils:sharedFileUtils():addSearchPath("res/")

    self:enterState("Menu")
end

function App:onEnterBackground()
end

function App:onEnterForeground()
end

return App
