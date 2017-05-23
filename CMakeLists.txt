cmake_minimum_required(VERSION 3.4.1)


project(QuickX)

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

	${CWD}/lib/third_party/android/prebuilt/SDL2/include
	${CWD}/lib/third_party/android/prebuilt/libcurl/include
	${CWD}/lib/third_party/android/prebuilt/libjpeg/include
	${CWD}/lib/third_party/android/prebuilt/libpng/include
	${CWD}/lib/third_party/android/prebuilt/libwebp/include
	${CWD}/lib/third_party/android/prebuilt/zlib/include
)

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


link_directories(
	${CWD}/lib/third_party/android/prebuilt/libcurl/libs/${ANDROID_ABI}/
	${CWD}/lib/third_party/android/prebuilt/libjpeg/libs/${ANDROID_ABI}/
	${CWD}/lib/third_party/android/prebuilt/libpng/libs/${ANDROID_ABI}/
	${CWD}/lib/third_party/android/prebuilt/libwebp/libs/${ANDROID_ABI}/
	${CWD}/lib/third_party/android/prebuilt/zlib/libs/${ANDROID_ABI}/
)


add_library(game SHARED template/sources/App.cpp)

target_link_libraries(game quickx 
curl jpeg png webp z
)
