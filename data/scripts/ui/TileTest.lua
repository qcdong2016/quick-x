
local M = class('SpriteTest', TestBase)

function M:setup()
    local tilemap = CCTMXTiledMap:create("orthogonal-test4.tmx"):addTo(self)
    local size = tilemap:getSize()
    tilemap:setPosition(ccp(-size.width/2, -size.height/2))
end


return M