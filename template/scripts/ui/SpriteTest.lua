
local M = class('SpriteTest', TestUI)

function M:setup()
    
    local frames = display.newFrames('AllSprites/CoinSpin%02d.png', 1, 8)
    local anim = display.newAnimation(frames, 0.03)

    director.resourceCache:addResource(anim)

    local icons = {}
    local function add(n)
        if n > 0 then
            for i=1, n do 
                local sprite = display.newSprite():addTo(self)
                    :pos(math.random(-display.cx, display.cx), math.random(-display.cy, display.cy))
                sprite:playAnimationForever(anim)
                table.insert(icons, sprite)
            end
        else
            for i=1, -n do
                local sp = table.remove(icons, 1)
                sp:removeSelf();
            end
        end

    end

    local btn = Button:create():addTo(self.root):scale(0.5):pos(display.cx - 100, 40)
    btn:loadTextureNormal('AllSprites/AddCoinButton.png')
    btn:onClicked(function() add(10) end)

    local btn = Button:create():addTo(self.root):scale(0.5):pos(display.cx + 100, 40)
    btn:loadTextureNormal('AllSprites/RemoveCoinButton.png')
    btn:onClicked(function() add(-10) end)
end


return M