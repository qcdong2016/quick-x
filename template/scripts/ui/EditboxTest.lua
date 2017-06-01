local M = class("TestEditbox", TestBase)

function M:setup()
    local c = CCLayerColor:create(ccc4(255, 255, 255, 255))
    c:setSize(CCSize(150, 50))
    c:setPosition(ccp(300, 300))
    self.root:addNode(c)

    local edit = EditBox:create()
    edit:setFontSize(30)
    edit:setPlaceHolder("input text")
    edit:setSize(CCSize(150, 50))
    edit:setPosition(ccp(300, 300))

    edit:onClicked(function()
        print("touched") 
    end)

    self.root:addChild(edit)
end

return M