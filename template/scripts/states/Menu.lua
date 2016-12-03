require('ui.TestUI')

local Menu = class("Menu", function()
    return display.newLayer()
end)

function Menu:ctor()

    local touchLayer = TouchGroup:create()
    self:addChild(touchLayer)

    self.touchLayer = touchLayer;


    local root = Widget:create()
    touchLayer:addWidget(root)

    local listView = ListView:create():addTo(root)
    listView:setSize(CCSize(300, display.height))
    listView:setPosition(ccp(display.cx - 150, 0))
    listView:setDirection(SCROLLVIEW_DIR_VERTICAL)
    listView:setLayoutType(LAYOUT_LINEAR_VERTICAL)
    listView:setClippingEnabled(true)
    listView:setClippingType(LAYOUT_CLIPPING_SCISSOR)

    local tests = {
        'ShaderTest',
        'SpineTest',
        'SpriteTest',
    }

    for i, v in ipairs(tests) do
        local label = Label:create()
        label:setFontSize(30)
        label:setText(v)
        label:onClicked(function()
            self:gotoTest(v)
        end)
        listView:pushBackCustomItem(label)
    end

end

function Menu:gotoTest(name)

    self.touchLayer:hide()

    local ui = UIMgr:open(name, name)
    ui:setup();
    ui:onClose(function()
        self.touchLayer:show()
    end)
end


return Menu
