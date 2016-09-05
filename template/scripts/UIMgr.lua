
local UIMgr = class('UIMgr', function()
	return display.newNode()
end)

function UIMgr:ctor()
	self:setupBlackLayer()

	self.show_layers = {}
end

function UIMgr:hideAll(ex)
    for k,v in pairs(self.show_layers) do
        if v.layer ~= ex then
            v.layer:setVisible(false);
        end
    end
end

function UIMgr:closeAll(ex)
	local tmp = {};
    for k,v in pairs(self.show_layers) do
        if v.layer ~= ex then
        	table.insert(tmp,v);
        end
    end
    while #tmp > 0 do
    	table.remove(tmp,1).layer:close();
    end
end

function UIMgr:showAll()
    for k,v in pairs(self.show_layers) do
        v.layer:setVisible(true);
    end
end

function UIMgr:close(instance)
	local old_top_info = self.show_layers[#self.show_layers]
	local close_info = self:findAndRemove(instance)
	local new_top_info = self.show_layers[#self.show_layers]

	self:resetAllLayer()

	--如果关闭的是最上层，那么应该通知新的最上层layer
	if new_top_info then
		new_top_info.layer:setVisible(true)

		if old_top_info == close_info and new_top_info.layer.onShowTop then
			new_top_info.layer:onShowTop()
		end
	end
end

function UIMgr:pop()
	local info = self.show_layers[#self.show_layers]
	if info then
		self:close(info.layer)
	end
end

function UIMgr:findAndRemove(instance)
	for i, v in ipairs(self.show_layers) do
		if instance == v.layer then
			return table.remove(self.show_layers, i)
		end
	end
end

--会根据名字存储实例，下次调用后不会再次创建实例
function UIMgr:pushByName(name, ...)
	local info

	for i, v in ipairs(self.show_layers) do
		if name == v.name then
			info = table.remove(self.show_layers, i)
			break
		end
	end

	if info then
		--重新添加
		local newinfo = self:push(info.layer, name, true)

		if info.layer.onShowTop then
			newTopLayer:onShowTop()
		end

		info.layer:setVisible(true)

		return info.layer
	end

	local inst = require(name).new(...)
	self:push(inst, name)

	return inst
end

function UIMgr:resetAllLayer()
	for i, v in ipairs(self.show_layers) do
		v.layer:setZOrder(i*2 + 2)
	end

	self:resetBlackLayer()
end

function UIMgr:push(instance, name, added)
	local last = self.show_layers[#self.show_layers]

	local info = {layer = instance, name = name}
	table.insert(self.show_layers, info)

	if not added then
		instance:addTo(self)
	end
	
	instance:setZOrder(#self.show_layers * 2 + 2)
	self:resetBlackLayer()

	if last then
		if info.layer.cover then
			last.layer:setVisible(false)
		end
		if last.onCovered then last:onCovered() end
	end
end

function UIMgr:resetBlackLayer(z)
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

function UIMgr:setupBlackLayer()
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
end

function UIMgr:setBlackLayerVisible(v)
	self.blackLayer_visible = v
	if not v then
		self.blackLayer:setVisible(v)
	else
		self.blackLayer_visible = nil
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