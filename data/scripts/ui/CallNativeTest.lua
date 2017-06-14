local M = class("CallNativeTest", TestBase)

function M:setup()
    local btn = Button:create():addTo(self)
    btn:loadTextureNormal('AllSprites/AddCoinButton.png')
    btn:onClicked(function() 
        print(callStaticMethod("org/quickx/QuickXPlayer", "sayHello"))
        print(callStaticMethod("org/quickx/QuickXPlayer", "sayHelloMap", {
                testStr = "world",
                testNum = 2,
                testBool = true,
                testFunc = function(msg) print(msg) end,
            }))
    end)
end

return M