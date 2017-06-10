local M = class("TestEditbox", TestBase)

function M:setup()
    local c = CCLayerColor:create(ccc4(122, 122, 122, 255))
    c:setSize(CCSize(400, 400))
    c:setAnchorPoint(ccp(0.5, 0.5))
    self:addChild(c)

    self:createEditBox():addTo(c):pos(-150, 100)
    self:createEditBox():addTo(c):pos(-150, -100)
end

function M:createEditBox()
    local bg = CCLayerColor:create(ccc4(100, 255, 100, 255))
    bg:setSize(CCSize(300, 80))
    bg:setAnchorPoint(0, 0.5)

    local edit = EditBox:create():addTo(bg)
    edit:setAnchorPoint(ccp(0, 0.5))
    edit:setSize(CCSize(300, 80))

    return bg
end

return M