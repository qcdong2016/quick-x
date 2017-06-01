
local M = class('SpriteTest', TestBase)

function M:setup()
    
   CCTMXTiledMap:create("orthogonal-test4.tmx"):addTo(self)
end


return M