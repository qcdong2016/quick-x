local M = class("CallNativeTest", TestBase)

function M:setup()
    local btn = Button:create():addTo(self)
    btn:loadTextureNormal('AllSprites/AddCoinButton.png')

    local className = nil

    if device.platform == 'ios' then
        className = "CCNativeIOS"
    elseif device.platform == 'android' then
        className = "org/quickx/QuickXPlayer"
    end

    btn:onClicked(function() 
        print(callStaticMethod(className, "sayHello"))
        print(callStaticMethod(className, "sayHelloMap", {
                testStr = "world",
                testNum = 2,
                testBool = true,
                testFunc = function(msg) print(msg) end,
            }))
    end)
end

return M