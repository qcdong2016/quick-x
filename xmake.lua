
local function find(arrar, value)
	for i, v in ipairs(array) do
		if v == value then
			return true
		end
	end	
end

local function merge(arr1, arr2)
	if not arr2 then return end
	if not arr1 then return arr2 end

	for i, v in ipairs(arr2) do
		table.insert(arr1, v)
	end

	return arr1
end

local function convert(t, name)
	local newt = {}
	for i, v in ipairs(t[name]) do
		newt[i] = 'lib/cocos2d-x/' .. t.name .. '/' .. v
	end

	t[name] = newt
end

local function each_call(array, func)
	for i, v in ipairs(array) do
		func(v)
	end
end	

local module_list = {}
function module(mod)
	module_list[mod.name] = mod
	table.insert(module_list, mod)

	if mod.name ~= 'core' then
		mod.deps = mod.deps or {}
		if not find(mod.deps, 'core') then
			table.insert(mod.deps, 'core')
		end
	end

	mod.includes = mod.includes or {}
	if not find(mod.includes, '.') then
		table.insert(mod.includes, '.')
	end
end

local function translate()
	for i, mod in ipairs(module_list) do
		convert(mod, "includes")
		convert(mod, "files")
	end
end


local function build()
	translate()

	for i, mod in ipairs(module_list) do
		target(mod.name)
		set_kind('static')

		for i, depname in ipairs(mod.deps) do
			local dp = module_list[depname]
			mod.includes = merge(mod.includes, dp.includes)
			mod.defines = merge(mod.defines, dp.defines)

			add_deps(depname)
		end

		each_call(mod.includes, add_includedirs)
		each_call(mod.files, add_files)
		each_call(mod.defines, add_defines)
	end
end

set_project('quick-x')
set_version("1.0")

set_languages("cxx11", "cxx11")

module {
    name = 'core',
    files = {
        "**.c*|platform/**",
        "platform/mac/**.m*",
    },

    includes = {
        'cocoa',
        'platform', 
        'platform/mac',
        'kazmath/include',
        '..',
        '../scripting',
        '../scripting/lua',
        '../scripting/tolua',
        '../scripting/cocoslua',
    },

    defines = { 
        'CC_TARGET_OS_MAC', 
        'USE_FILE32API' 
    },

    framework = {
        'OpenGL',
        'Foundation',
        'AppKit',
    }
}

module {
    name = 'actions',
	files = {
		"*.cpp",
	},
}

module {
	name = 'audio',
	files = {
		'mac/*.m*',
	},
}

module {
	name = "imgui",
	files = {
		"**.*c*"
	},
}

module {
	name = "network",

	files = {
		'*.c*|*Android.cpp|*Win32.cpp',
		'*.mm',
		'**.m',
	},
}

module {
	name = 'particle',
	files = {
		'*.c*',	
	},
}

module {
	name = 'scripting',
	files = {
		'lua/*.c',
		'lua_extensions/**.c*|*/wsocket.c',
		'tolua/**.c*',
		'*.c*',
	},

	includes = {
		'lua',
		'tolua',
		'lua_extensions',
	},
}

module {
	name = 'spine',
	files = {
		'**.c*',	
	},
}

module {
	name = 'tilemap',
	files = {
		'*.c*',	
	},
}

build()

