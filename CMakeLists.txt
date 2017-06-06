cmake_minimum_required(VERSION 3.4.1)


project(QuickX)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CWD ${CMAKE_CURRENT_LIST_DIR})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(ThirdInc 
	${CWD}/lib/3rd
	${CWD}/lib/3rd/stb
	${CWD}/lib/3rd/imgui
	${CWD}/lib/3rd/SDL2/include
	${CWD}/lib/3rd/tinyxml2/
	${CWD}/lib/3rd/tolua/
	${CWD}/lib/3rd/lua/

	${CWD}/lib/cocos2d-x/
	${CWD}/lib/cocos2d-x/core
	${CWD}/lib/cocos2d-x/core/platform
)

if (MSVC)
	list(APPEND ThirdInc
		${CWD}/lib/3rd/zlib/
		${CWD}/lib/3rd/glew/
	)
endif()

macro (make_source_group)
	foreach(src_file ${SOURCE_FILES})
	    get_filename_component (PATH ${src_file} PATH)
	    string (REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" PATH ${PATH})
	    string (REPLACE / \\ PATH ${PATH})
	    source_group ("Source Files\\${PATH}" FILES ${src_file})
	endforeach()
endmacro()

macro (library name)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    include_directories(${INCLUDE_DIRS} ${ThirdInc})
    add_library(${name} STATIC ${SOURCE_FILES})
endmacro()



macro (lazy_setup)
	get_filename_component(BaseName ${CMAKE_CURRENT_LIST_DIR}  NAME_WE)
	file (GLOB SOURCE_FILES ${CMAKE_CURRENT_LIST_DIR}/*.cpp ${CMAKE_CURRENT_LIST_DIR}/*.c ${CMAKE_CURRENT_LIST_DIR}/*.h)
	set(INCLUDE_DIRS ${CMAKE_CURRENT_LIST_DIR})
	library(${BaseName})
endmacro()

add_subdirectory(lib/3rd)
add_subdirectory(lib/cocos2d-x)

include_directories(${ThirdInc})

if(ANDROID)
	add_library(game SHARED lib/App.cpp)
else()
	add_executable(game lib/App.cpp)
endif()

target_link_libraries(game quickx)
