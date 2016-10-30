local UILayer = require "UILayer"

local HelloUI = class('HelloUI', UILayer)

function HelloUI:ctor()
    UILayer.ctor(self)
    
    local hello = display.newSprite('HelloWorld.png'):addTo(self)

    hello:runAction(CCRepeatForever:create(CCSequence:create(
        CCScaleTo:create(3, 0.5),
        CCScaleTo:create(1, 1)
    )))

    hello:runAction(CCScaleTo:create(3, 0.5))

    ui.newTTFLabel({size= 25, text = 'Hello World'}):addTo(self, 1);
    
    -- imgui.addDraw(function()
    --     imgui.Button('hello lua')
    -- end)

    -- imgui.addDraw(function()
    --     imgui.Button('hello lua')
    -- end)

     -- CCLayerColor:create(ccc4(0,55,0,255)):addTo(self)
     display.newRect(100,100):addTo(self)

    local spine = SkeletonAnimation:createWithFile("spineboy.json", "spineboy.atlas", 1)
        :addTo(self)
        :pos(-200+math.random(-100,100), -display.cy)
    spine:setAnimation(0, 'run', true)

    local touchLayer = TouchGroup:create():pos(-display.cx, -display.cy)
    self:addChild(touchLayer)

    -- function Button(text, func)
    --     local btn = Label:create()
    --     btn:ignoreContentAdaptWithSize(false)
    --     btn:setSize(CCSize(500, 50))
    --     btn:setFontSize(40)
    --     btn:setText(text)
    --     btn:setTouchEnabled(true)
    --     btn:setTextHorizontalAlignment(kCCTextAlignmentCenter)
    --     btn:setTextVerticalAlignment(kCCVerticalTextAlignmentCenter)
    --     btn:addTouchEventListener(function(_, type)
    --         if type == TOUCH_EVENT_ENDED then
    --             if func then func(btn); end
    --         end
    --     end)
    --     touchLayer:addWidget(btn)
    --     return btn
    -- end

    -- local btn = Button('send Event 1', function(self)
    --     self:sendEvent(1)
    -- end):pos(100, 100)

    -- local btn2 = Button('send Event 2', function(self)
    --     self:sendEvent(2)
    -- end):pos(100, 200)


    -- local btn3 = Button('remove 1', function(self)
    --     print('unsubscribeFromAllEvents')
    --     -- btn:removeSelf()
    --     self:unsubscribeFromEvent(1)
    -- end):pos(300, 100)

    -- btn:subscribeToEvent(1, function(type, data)
    --     print('onEvent 1,1')
    -- end)

    -- btn2:subscribeToEvent(2, function(type, data)
    --     print('onEvent 2')
    -- end)

    -- btn3:subscribeToEvent(1, function(type, data)
    --     print('onEvent 3,1')
    -- end)

    -- btn3:subscribeToEvent(btn, 1, function(type, data)
    --     print('onEvent 3,11')
    -- end)

    local root = Widget:create()
    touchLayer:addWidget(root)
    local listView = ListView:create():addTo(root)
    listView:setSize(CCSize(200, display.height - 150))
    listView:setPosition(ccp(display.width - 200, 100))
    listView:setDirection(SCROLLVIEW_DIR_VERTICAL)
    listView:setLayoutType(LAYOUT_LINEAR_VERTICAL)
    listView:setClippingEnabled(false)

    local filters = require "shaders"
    local currentLabel = nil
    local currentMaterial = nil
    for k, v in pairs(filters) do
        local label = Label:create()
        label:setFontSize(30)
        label:setText(k)
        label:onClicked(function()
            print('use', k)
            hello:setMaterial(v)
            spine:setMaterial(v)
            currentMaterial = v;
            if currentLabel then
                currentLabel:setColor(ccc3(255,255,255))
            end
            currentLabel = label
            currentLabel:setColor(ccc3(255,0,0))

        end)
        listView:pushBackCustomItem(label)
    end

    if imgui then
        imgui.addDraw(function()
            if currentMaterial then
                for i, v in ipairs(currentMaterial.editors) do
                    v()
                end
            end
        end)
    end
end

return HelloUI