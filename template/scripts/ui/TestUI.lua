local UILayer = require "UILayer"

TestUI = class('TestUI', UILayer)

function TestUI:ctor()
    UILayer.ctor(self)

    local touchLayer = TouchGroup:create():pos(-display.cx, -display.cy)
    self:addChild(touchLayer)

    local root = Widget:create()
    touchLayer:addWidget(root)
    local listView = ListView:create():addTo(root)
    listView:setSize(CCSize(200, display.height))
    listView:setPosition(ccp(display.width - 200, 0))
    listView:setDirection(SCROLLVIEW_DIR_VERTICAL)
    listView:setLayoutType(LAYOUT_LINEAR_VERTICAL)

    listView:setClippingEnabled(true)
    listView:setClippingType(LAYOUT_CLIPPING_SCISSOR)

    self.listView = listView;
    self.touchLayer = touchLayer
    self.root = root

    local btn = Button:create():addTo(root):scale(0.5)
    btn:loadTextureNormal('AllSprites/ExitButton.png')
    btn:setPosition(ccp(40, display.top -40))
    btn:onClicked(function()
        self:close();
    end)
end

function TestUI:pushButton(name, func)
    local label = Label:create()
    label:setFontSize(30)
    label:setText(name)
    label:onClicked(func)
    self.listView:pushBackCustomItem(label)
end

return TestUI