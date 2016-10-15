
luaj = require(cc.PACKAGE_NAME .. ".luaj")

function io.exists(path)
    return FileSystem:isFileExist(path)
end

function io.readfile(path)
    return FileSystem::readAll(path)
end

function device.showAlertAndroid(title, message, buttonLabels, listener)
end
