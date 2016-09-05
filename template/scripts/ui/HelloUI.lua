local UILayer = require "UILayer"

local HelloUI = class('HelloUI', UILayer)

function HelloUI:ctor()
    UILayer.ctor(self)
    
    display.newSprite('HelloWorld.png'):addTo(self)
    ui.newTTFLabel({size= 25, text = 'Hello World'}):addTo(self, 1);
end

return HelloUI