
local M = class('ShaderTest', TestUI)

function M:setup()
    local spine = SkeletonAnimation:create("spineboy.json", "spineboy.atlas", 1)
        :addTo(self)
        :pos(-100, -display.cy)
    spine:setAnimation(0, 'run', true)

    spine:subscribeToEvent('SpineEvent', function(e)
        print('Event', e.eventName)
        local label = Label:create()
        label:setFontSize(30)
        label:addTo(self.root)
        label:center()
        label:setText(string.format('event：[%s][%s]', e.eventName, e.stringValue))

        label:runAction(CCSequence:create(
            CCMoveTo:create(1, ccp(0, 100)),
            CCRemoveSelf:create()
        ))
    end)
    
    local function Animation(name)
        self:pushButton(name, function() spine:setAnimation(0, name, true) end)
    end

    Animation('run')
    Animation('death')
    Animation('hit')
    Animation('idle')
    Animation('jump')
    Animation('shoot')
    Animation('walk')
end

return M