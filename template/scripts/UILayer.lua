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

    self.nextListenerHandleIndex_ = 0
    self.listeners_ = {}

    self:setNodeEventEnabled(true)
end

function UILayer:close()
    local cname = self.__cname

    UIMgr:close(self)
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


function UILayer:addEventListener(eventName, listener, tag)
    assert(type(eventName) == "string" and eventName ~= "",
        "UILayer:addEventListener() - invalid eventName")
    eventName = string.upper(eventName)
    if self.listeners_[eventName] == nil then
        self.listeners_[eventName] = {}
    end

    local ttag = type(tag)
    if ttag == "table" or ttag == "userdata" then
        PRINT_DEPRECATED("UILayer:addEventListener(eventName, listener, target) is deprecated, please use UILayer:addEventListener(eventName, handler(target, listener), tag)")
        listener = handler(tag, listener)
        tag = ""
    end

    self.nextListenerHandleIndex_ = self.nextListenerHandleIndex_ + 1
    local handle = tostring(self.nextListenerHandleIndex_)
    tag = tag or ""
    self.listeners_[eventName][handle] = {listener, tag}

    if DEBUG > 1 then
        printInfo("%s [UILayer] addEventListener() - event: %s, handle: %s, tag: %s", tostring(self.target_), eventName, handle, tostring(tag))
    end

    return handle
end

function UILayer:dispatchEvent(event)
    event.name = string.upper(tostring(event.name))
    local eventName = event.name
    if DEBUG > 1 then
        printInfo("%s [UILayer] dispatchEvent() - event %s", tostring(self.target_), eventName)
    end

    if self.listeners_[eventName] == nil then return end
    event.target = self.target_
    event.stop_ = false
    event.stop = function(self)
        self.stop_ = true
    end

    for handle, listener in pairs(self.listeners_[eventName]) do
        if DEBUG > 1 then
            printInfo("%s [UILayer] dispatchEvent() - dispatching event %s to listener %s", tostring(self.target_), eventName, handle)
        end
        -- listener[1] = listener
        -- listener[2] = tag
        listener[1](event)
        if event.stop_ then
            if DEBUG > 1 then
                printInfo("%s [UILayer] dispatchEvent() - break dispatching for event %s", tostring(self.target_), eventName)
            end
            break
        end
    end

    return self.target_
end

return UILayer