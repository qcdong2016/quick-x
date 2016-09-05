local Menu = class("Menu", function() 
    return display.newLayer()
end)

function Menu:ctor()
    display.newColorLayer(ccc4(255,255,255,155)):addTo(self, -1)

    require("ui.HelloUI").new():show()
end


return Menu