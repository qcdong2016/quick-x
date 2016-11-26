
local M = class('ShaderTest', TestUI)

function M:setup()
    local spine = SkeletonAnimation:create("spineboy.json", "spineboy.atlas", 1)
        :addTo(self)
        :pos(-100, -display.cy)
    spine:setAnimation(0, 'run', true)

    spine:subscribeToEvent('SpineEvent', function(e)
        print('Event', e.eventName)
        local label = Label:create():addTo(self):pos(0, -display.cy)
        label:setFontSize(30)
        label:setText(string.format('event：[%s]', e.eventName))

        label:runAction(CCSequence:create(
            CCMoveBy:create(1, ccp(0, 100)),
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