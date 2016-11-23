local platform = 'android'
if is_plat('macosx') then
	platform = 'mac'
elseif is_plat('iphoneos') then
	platform = 'ios'
elseif is_plat('windows') then
	platform = 'win32'
end

local function find(arrar, value)
	for i, v in ipairs(array) do
		if v == value then
			return i
		end
	end	
end

local function merge(arr1, arr2)
	if not arr2 then return arr1 end
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
local executable_list = {}

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

function executable(mod)
	executable_list[mod.name] = mod
	table.insert(executable_list, mod)
	mod.deps = mod.deps or {}

	for i, m in ipairs(module_list) do
		table.insert(mod.deps, m.name)
	end
end


local function plat_specify(mod, name)
	if mod[name] then
		if mod[name][platform] then
			for i, v in ipairs(mod[name][platform]) do
				table.insert(mod[name], v)
			end
		end
	end
end


local function translate()
	for i, mod in ipairs(module_list) do
		plat_specify(mod, 'includes')
		plat_specify(mod, 'files')
		plat_specify(mod, 'defines')
		convert(mod, "includes")
		convert(mod, "files")
	end

	for i, mod in ipairs(executable_list) do
		plat_specify(mod, 'includes')
		plat_specify(mod, 'files')
		plat_specify(mod, 'defines')
	end
end

local function build()
	translate()

	local mark = {}
	local function build(mod)
		if mark[mod.name] then
			return;
		end
		mark[mod.name] = true;

		target(mod.name)
		set_kind(mod.kind or 'static')

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

	build(module_list['core'])

	for i, mod in ipairs(executable_list) do
		build(mod)
	end

	for i, mod in ipairs(module_list) do
		build(mod)
	end
end

set_project('quick-x')
set_version("1.0")
set_languages("gnuxx11", "gnuxx11")


module {
    name = 'core',
    files = {
        "**.c*|platform/**",
        mac = { "platform/mac/**.m*", },
        android = { "platform/android/**.c*", },
    },

    includes = {

        'cocoa',
        'platform', 
        'platform/'..platform,
        'kazmath/include',
        '..',
        '../scripting',
        '../scripting/lua',
        '../scripting/tolua',
        '../scripting/cocoslua',

        android = {
	        '../../third_party/android/prebuilt/libpng/include',
	        '../../third_party/android/prebuilt/libjpeg/include',
	        '../../third_party/android/prebuilt/libcurl/include',
	        '../../third_party/android/prebuilt/libwebp/include',
	        '../../third_party/android/prebuilt/zlib/include',
	    },
    },

    defines = { 
    	mac = {
        	'CC_TARGET_OS_MAC', 
	    },
	    android = {
	        'ANDROID',
		},

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
		
		mac = { 'mac/**.m*', },
		android = { 'android/**.c*|*/opensl/**.*', },
	},
}

if platform ~= 'android' then
	module {
		name = "imgui",
		files = {
			"**.*c*"
		},
	}
end

module {
	name = "network",

	files = {
		mac = {
			'*.c*|*Android.cpp|*Win32.cpp',
			'*.mm',
			'**.m',
		},
		android = {
			'**.c*|*Win32.cpp',
		},
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

executable {
	name = 'player',
	kind = 'shared',
	files = {
		'template/proj.android/jni/**.c*',
		'template/sources/**.c*',
	},
	includes = {
		'template/sources',
	}
}



build()

