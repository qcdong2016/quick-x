


file (GLOB SOURCE_FILES 
	core/base/*.cpp 
	core/cocoa/*.cpp 
	core/cocoa/data_support/*.cpp 
	core/engine/*.cpp
	core/graphics/*.cpp
	core/shaders/*.cpp
	core/nodes/*.cpp
	core/platform/*.cpp

	core/resources/*.cpp
	core/text_input_node/*.cpp
	core/textures/*.cpp
	core/touch_dispatcher/*.cpp
	core/*.cpp
	core/*.c

	actions/*.cpp
	crypto/*.cpp crypto/*.c
	imgui/*.cpp
	#network/*.cpp
	particle/*.cpp
	scripting/*.cpp scripting/*.c
	spine/*.cpp
	tilemap/*.cpp
	zip/*.cpp
	misc/*.cpp

	ui/webview/*.cpp 
	ui/ScrollWidget/*.cpp 
	ui/Layouts/*.cpp
	ui/EditBox/*.cpp
	ui/*.cpp
)

if (MSVC)
	file (GLOB PLAT_SOURCE_FILES 
		audio/win32/*.cpp
		core/platform/win32/*.cpp
	)
	list(APPEND SOURCE_FILES ${PLAT_SOURCE_FILES})
	add_definitions(-DUNICODE
					-D_UNICODE
					-D_CRT_SECURE_NO_WARNINGS
					-D_SCL_SECURE_NO_WARNINGS)
else()
	file (GLOB PLAT_SOURCE_FILES 
		core/platform/android/*.cpp
		core/platform/android/jni/*.cpp
		audio/android/*.cpp
	)
	list(APPEND SOURCE_FILES ${PLAT_SOURCE_FILES})
endif()

set (INCLUDE_DIRS
	${CMAKE_CURRENT_LIST_DIR}
)

make_source_group()
library(quickx)

target_link_libraries(quickx
imgui kazmath lua lua_extensions SDL2 spine stb tinyxml2 tolua 
)

if (MSVC)#win32
	target_link_libraries(quickx GLEW zlib
		ws2_32
		wsock32
		winmm
		wldap32
		iphlpapi
		shlwapi
		crypt32
	)
else()#android
	target_link_libraries(quickx GLESv1_CM GLESv2 log android z)
endif()