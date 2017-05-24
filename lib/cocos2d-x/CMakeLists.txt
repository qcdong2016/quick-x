


file (GLOB SOURCE_FILES 
	core/base/*.cpp 
	core/cocoa/*.cpp 
	core/engine/*.cpp
	core/graphics/*.cpp
	core/shaders/*.cpp
	core/nodes/*.cpp
	core/platform/*.cpp
	core/platform/android/*.cpp
	core/platform/android/jni/*.cpp
	core/resources/*.cpp
	core/text_input_node/*.cpp
	core/textures/*.cpp
	core/touch_dispatcher/*.cpp
	core/*.cpp
	core/*.c

	actions/*.cpp
	crypto/*.cpp crypto/*.c
	imgui/*.cpp
	network/*.cpp
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

	audio/android/*.cpp
)

set (INCLUDE_DIRS
	${CMAKE_CURRENT_LIST_DIR}
)

library(quickx)

target_link_libraries(quickx
imgui kazmath lua lua_extensions SDL2 spine stb tinyxml2 tolua 
GLESv1_CM GLESv2 log android z
)