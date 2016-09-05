local W = Widget
local TOUCH_EVENT_ENDED = TOUCH_EVENT_ENDED

function W:onClicked(func)
    self:addTouchEventListener(function(_, type)
        if type == TOUCH_EVENT_ENDED then
            func(self);
        end
    end)
end