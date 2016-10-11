local UILayer = require "UILayer"

local HelloUI = class('HelloUI', UILayer)

function HelloUI:ctor()
    UILayer.ctor(self)
    
    local hello = display.newSprite('HelloWorld.png'):addTo(self)

    hello:runAction(CCRepeatForever:create(transition.sequence({
        CCScaleTo:create(3, 0.5),
        CCScaleTo:create(1, 1)
    })))

    hello:runAction(CCScaleTo:create(3, 0.5))

    ui.newTTFLabel({size= 25, text = 'Hello World'}):addTo(self, 1);
end

return HelloUI