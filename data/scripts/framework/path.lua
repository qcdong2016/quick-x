if not lfs then
    require "lfs"
end

local path = {}

path.chdir = lfs.chdir
path.touch = lfs.touch
path.rename = os.rename
path.remove = os.remove

function path.getatime(file)
    return lfs.attributes(file, 'access')
end

function path.getmtime(file)
    return lfs.attributes(file, 'modification')
end

function path.hasext(fname, exts)
    local fext = path.getext(fname):lower()
    if type(exts) == "table" then
        for _, ext in pairs(exts) do
            if fext == ext then
                return true
            end
        end
        return false
    else
        return (fext == exts)
    end
end

function path.getname(p)
    local i = p:findlast("[/\\]")
    if i then
        return p:sub(i + 1)
    else
        return p
    end
end

function path.getext(p)
    local i = p:findlast(".", true)
    if i then
        return p:sub(i)
    else
        return ""
    end
end

function path.getdir(p)
    local i = p:findlast("[/\\]")
    if i then
        if i > 1 then i = i - 1 end
        return p:sub(1, i)
    else
        return "."
    end
end

function path.getbasename(p)
    local name = path.getname(p)
    local i = name:findlast(".", true)
    if i then
        return name:sub(1, i - 1)
    else
        return name
    end
end

function path.join(p1, p2, ...)
    if not p2 then return p1 end
    if #p1 == 0 then return p2 end

    local s = string.char(string.byte(p1, #p1))

    if s == '/' or s == '\\' then
        return path.join(p1 .. p2, ...)
    else
        return path.join(p1..'/'..p2, ...)
    end
end

function path.isfile(f)
    local ff = io.open(f, 'r')
    if not ff then
        return false
    else
        ff:close()
    end

    return true
end

function path.isdir(f)
    local attr = lfs.attributes(f)
    return (attr and attr.mode== 'directory')
end

function path.normalize(p)
    return (string.gsub(p, '\\', '/'))
end

function path.getcwd()
    return lfs.currentdir()
end

function path.mkdir(p)
    return lfs.mkdir(p)
end

local function copy_file(from, to, override)
    local fromsize = lfs.attributes(from, 'size')

    if path.isfile(to) and not override then
        return false
    end

    local f_from = io.open(from, 'rb')
    local f_to = io.open(to, 'wb')

    if not f_from or not f_to then
        return false
    end

    local noteof = true

    while true do
        local data = f_from:read(4096)
        if not data then
            break
        end
        f_to:write(data)
    end

    f_to:close()
    f_from:close()
end

function path.copy(from, to, override)
    if path.isfile(from) then
        copy_file(from, to, override)
    else
        print('not imp')
    end
end

function path.mkdirs(dir)
    local pattern = "[\\/]"
    local pos = 0

    for st, sp in function() return dir:find(pattern, pos, plain) end do
        local d = dir:sub(0, st-1)
        if not path.isdir(d) then
            path.mkdir(d)
        end
        pos = sp + 1
    end

    if not path.isdir(dir) then
        path.mkdir(dir)
    end
end

function path.listdir(d, r)
    local ret = {}

    local function do_list(d, r)
        for file in lfs.dir(d) do
            if file ~= "." and file ~= ".." then
                local f = path.join(d, file)
                if path.isdir(f) then
                    table.insert(ret, f)
            if r then
                        do_list(f, r)
            end
                end
            end
        end
    end

    do_list(d, r)

    return ret
end

function path.listfile(d, r, exts, exclude)
    local add = table.insert
    if exts then
        if exclude then
            add = function(ret, f)
                if not path.hasext(f, exts) then
                    table.insert(ret, f)
                end
            end
        else
            add = function(ret, f)
                if path.hasext(f, exts) then
                    table.insert(ret, f)
                end
            end
        end
    end

    local ret = {}
    local function do_list(d,r)
        for file in lfs.dir(d) do
            if file ~= "." and file ~= ".." then
                local f = path.join(d, file)
                if not path.isdir(f) then
                    add(ret, f)
                elseif r then
                    do_list(f, r)
                end
            end
        end
    end
    do_list(d,r)
    return ret
end

function path.rmdir(d)
    local files = path.listfile(d, true)
    
    for i, v in ipairs(files) do
        os.remove(v)
    end

    local dirs = path.listdir(d, true)

    table.sort(dirs, function(a, b) return #a > #b end)

    for i, v in ipairs(dirs) do
        lfs.rmdir(v)
    end

    lfs.rmdir(d)
end


return path
