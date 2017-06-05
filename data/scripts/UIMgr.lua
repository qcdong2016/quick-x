
local UIMgr = class('UIMgr', function()
    return display.newNode()
end)

--[[
    onShow  -> when first show or bring to top
    onClose -> 
    onHide  -> another ui pushed at top
]]

function UIMgr:ctor()
    self.blackLayer = CCLayerColor:create()
    self.blackLayer:setColor(ccc3(0, 0, 0))
    self.blackLayer:setOpacity(210)
    self.blackLayer:setZOrder(-1)
    self.blackLayer:setVisible(false)
    self:addChild(self.blackLayer)

    self.blackLayer:addNodeEventListener(cc.NODE_TOUCH_EVENT, function(event)
        local info = self.show_layers[#self.show_layers]
        if info and info.layer and info.layer.onTouchOutSide then
            info.layer:onTouchOutSide ()
        end
        return true;
    end)

    self.show_layers = {}
end

function UIMgr:_reorderBlack(z)
    local count = #self.show_layers
    local bgVisible = (count ~= 0)
    local newz = z and z or count*2+1

    if self.blackLayer_visible ~= nil then
        self.blackLayer:setVisible(self.blackLayer_visible)
    else
        self.blackLayer:setVisible(bgVisible)
    end
    self.blackLayer:setTouchEnabled(bgVisible)
    self.blackLayer:setZOrder(newz)
end

function UIMgr:open(name)
    local inst = require(name).new()
    self:push(inst)
    return inst
end

function UIMgr:close(instance)
    local old = self.show_layers[#self.show_layers]

    local closed = nil
    for i, v in ipairs(self.show_layers) do
        if instance == v.layer then
            closed = table.remove(self.show_layers, i)
            break
        end
    end

    if not closed then
        return
    end

    local new = self.show_layers[#self.show_layers]

    self:resetAllLayer()

    if new then
        new.layer:setVisible(true)

        -- if instance at top
        if old == closed and new.layer.onShow then
            new.layer:onShow()
        end
    end

    if closed.layer.onClose then
        closed.layer:onClose()
    end

    closed.layer:removeSelf()
end

function UIMgr:resetAllLayer()
    for i, v in ipairs(self.show_layers) do
        v.layer:setZOrder(i*2 + 2)
    end

    self:_reorderBlack()
end


function UIMgr:pop()
    local info = self.show_layers[#self.show_layers]
    if info then
        self:close(info.layer)
    end
end

function UIMgr:push(instance, name, added)
    local last = self.show_layers[#self.show_layers]

    local info = {layer = instance, name = name}
    table.insert(self.show_layers, info)

    if not added then
        instance:addTo(self)
    end

    if instance.onShow then
        instance:onShow()
    end

    instance:center()
    instance:setZOrder(#self.show_layers * 2 + 2)
    self:_reorderBlack()

    if last then
        if info.layer.cover then
            last.layer:setVisible(false)
        end
        if last.onHide then last:onHide() end
    end
end

function UIMgr:clear()
    for i, v in ipairs(self.show_layers) do
        v.layer:removeSelf()
    end

    self.show_layers = {}
    self:resetBlackLayer()
end

function UIMgr:isEmpty()
    return (#self.show_layers == 0)
end

function UIMgr:hasUI(name)
    for i, v in ipairs(self.show_layers) do
        if v.layer.__cname == name then
            return v.layer
        end
    end
end

return UIMgr
