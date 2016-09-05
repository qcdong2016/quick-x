local UILayer = class('UILayer', function() 
    return display.newNode()
end)

local Event = {}

Event.Close = 'Close'
Event.Show = 'Show'

UILayer.Event = Event

function UILayer:ctor()
    -- 覆盖其他界面
    self.cover = true

    cc(self):addComponent("components.behavior.EventProtocol"):exportMethods()
    self:setNodeEventEnabled(true)
end

function UILayer:close()
    local cname = self.__cname

    UIMgr:close(self)
    self:removeAllSocketListenner()
    self:removeAllGameEvents()
    self:dispatchEvent({name=Event.Close});
    self:removeSelf()

    print('close ui', cname)
end

function UILayer:show()
    if self.isClose then
        return nil
    end
    UIMgr:push(self)
    self:center()
    
    self:dispatchEvent({name=Event.Show});
    print('show ui', self.__cname)
    return self
end

function UILayer:onClose(func)
    self:addEventListener(Event.Close, func)
    return self
end

function UILayer:onShow(func)
    self:addEventListener(Event.Show, func)
    return self
end

return UILayer