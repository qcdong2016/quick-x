TestBase = class('TestBase', function()
    return display.newNode()
end)

function TestBase:ctor(name)
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
        UIMgr:close(self)
    end)

    local label = LabelBMFont:create()
        :addTo(self, 999)
        :pos(0, display.cy - 40)
    label:setFntFile('UIFont.fnt')
    label:setText(name)
end

function TestBase:pushButton(name, func)
    local label = Label:create()
    label:setFontSize(30)
    label:setText(name)
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
    }

    for i, v in ipairs(tests) do
        local label = Label:create()
        label:setFontSize(30)
        label:setText(v)
        label:onClicked(function() self:gotoTest(v) end)
        listView:pushBackCustomItem(label)
    end
end

function Menu:gotoTest(name)

    self.touchLayer:hide()

    local ui = UIMgr:open("ui." .. name)
    ui:setup();
    ui.onClose = function()
        self.touchLayer:show()
    end
end

return Menu
