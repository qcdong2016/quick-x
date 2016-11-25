
local M = class('ShaderTest', TestUI)

function M:setup()
    local hello = display.newSprite('HelloWorld.png'):addTo(self)

    local filters = require "shaders"
    local curRender = nil
    for k, v in pairs(filters) do
        self:pushButton(k, function()
            print('use', k)
            hello:setRenderState(v)
            curRender = v;
        end)
    end

    if imgui then
        imgui.addDraw(function()
            if curRender then
                for i, v in ipairs(curRender.editors) do
                    v()
                end
            end
        end)
    end

    local time = 0
    self:scheduleUpdate_()
    self:addNodeEventListener(cc.NODE_ENTER_FRAME_EVENT, function(dt)
        time = time + dt
        filters.Wave:set1f('u_time', time)
    end)
end

return M