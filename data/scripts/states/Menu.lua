TestBase = class('TestBase', function()
    return display.newNode()
end)

function TestBase:ctor(name)
    local btn = Button:create():addTo(self, 999):scale(0.5)
    btn:loadTextureNormal('AllSprites/ExitButton.png')
    btn:setPosition(ccp(-display.cx + 40, display.cy -40))
    btn:onClicked(function()
        UIMgr:close(self)
    end)

    local label = LabelBMFont:create():addTo(self, 999):pos(0, display.cy - 40)
    label:setFntFile('UIFont.fnt')
    label:setText(name)
end

function TestBase:setupListView()
    local root = Widget:create():addTo(self)

    local listView = ListView:create():addTo(self)
    listView:setSize(CCSize(200, display.height))
    listView:setPosition(ccp(display.width - 200, 0))
    listView:setDirection(SCROLLVIEW_DIR_VERTICAL)
    listView:setLayoutType(LAYOUT_LINEAR_VERTICAL)

    listView:setClippingEnabled(true)
    listView:setClippingType(LAYOUT_CLIPPING_SCISSOR)

    self.listView = listView;
end

function TestBase:pushButton(name, func)
    local label = Label:create()
    label:setFontSize(30)
    label:setText(name)
    label:setName(name)
    label:onClicked(func)
    self.listView:pushBackCustomItem(label)
end
--------------------------------------------------

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
        'TileTest',
        'EditboxTest',
        'Game2048',
        'GamePopstar',
    }

    for i, v in ipairs(tests) do
        local label = Label:create()
        label:setFontSize(50)
        label:setText(v)
        label:setName(v)
        label:onClicked(function() self:gotoTest(v) end)
        listView:pushBackCustomItem(label)
    end
end

function Menu:gotoTest(name)

    self.touchLayer:hide()

    local ui = require("ui."..name).new(name)
    UIMgr:push(ui)
    ui:setup();
    ui.onClose = function()
        self.touchLayer:show()
    end
end

return Menu
