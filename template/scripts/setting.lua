

local function encode(str, key)
	return CCCrypto:encodeBase64(str, #str)
end

local function decode(str, key)
	return CCCrypto:decodeBase64(str)
end

local function load(filename, key)

	local file = io.open(device.writablePath..filename, "rb")
	if file then
	    local content = file:read("*a")
	    io.close(file)

	    if not content or content == "" then
	    	return
	    end

	    local f = nil
	    local function doload()
	    	f = loadstring(decode(content, key))
	    end
	    local function onerror(e)
	    	print("读取本地数据发生错误")
	    	f = nil
	    end
	    local function doload_without_decode()
	    	f = loadstring(content)
	    end

	    xpcall(doload, onerror)
	    if f then
	    	return f()
	    else--如果采用加密方式读取失败，则再用普通方式读取一次。
	    	xpcall(doload_without_decode, onerror)
	    	if f then
	    		return f()
	    	end
	    end
	end

	return  {}
end

local function save(t, filename, key)
	print('saved', filename)

	local str = table.show(t, "local data")
	str = str.."\nreturn data"

	local f = io.open(device.writablePath..filename, "w")
	f:write(encode(str), key)
	f:close()
end


local S = class('setting')

function S:ctor(filename, key)
	self.filename_ = filename
	self.key_ = key

	self.data = load(filename, key)
end

function S:save()
	save(self.data, self.filename_, self.key_)
end

function S:set(k, v)
	self.data[k] = v
end

function S:get(k)
	return self.data[k]
end

return S