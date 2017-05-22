cmake_minimum_required(VERSION 3.6)
file(GLOB C_FILES
    src/*.c
    src/atomic/*.c
    src/audio/*.c
    src/cpuinfo/*.c
    src/dynapi/*.c
    src/events/*.c
    src/file/*.c
    src/libm/*.c
    src/render/*.c
    src/render/*/*.c
    src/stdlib/*.c
    src/thread/*.c
    src/timer/*.c
    src/video/*.c
    src/joystick/*.c 
    src/haptic/*.c 
  )

include (CheckIncludeFiles)

if (WIN32)
    # New MinGW versions may evaluate whether to use A or W postfix on functions before SDL gets to define UNICODE on its own,
    # so make sure it is already defined
    if (MINGW)
        add_definitions (-DUNICODE=1)
    endif ()
    CHECK_INCLUDE_FILES (wbemcli.h HAVE_WBEMCLI_H)
    if (HAVE_WBEMCLI_H)
        add_definitions (-DSDL_JOYSTICK_DINPUT)
        add_definitions (-DSDL_HAPTIC_DINPUT)
        set (HAPTIC_DRIVER windows)
    else ()
        message (STATUS "Building SDL without DX joystick support due to missing wbemcli.h")
        message (STATUS "For MSVC, get it from Windows 7 SDK. For MinGW, get it from eg. Wine sources or from MinGW-w64")
        add_definitions (-DSDL_JOYSTICK_WINMM)
        add_definitions (-DSDL_HAPTIC_DUMMY)
        set (HAPTIC_DRIVER dummy)
    endif ()

    file (GLOB SYS_C_FILES
        src/audio/directsound/*.c 
        src/haptic/${HAPTIC_DRIVER}/*.c 
        src/joystick/windows/*.c 
        src/core/windows/*.c 
        src/loadso/windows/*.c
        src/power/windows/*.c 
        src/thread/windows/*.c 
        src/thread/generic/SDL_syscond.c 
        src/timer/windows/*.c 
        src/video/windows/*.c 
        src/joystick/dummy/*.c
        src/filesystem/windows/*.c
    )
elseif (IOS)
    file (GLOB SYS_C_FILES
        src/audio/coreaudio/*.c src/file/cocoa/*.m src/joystick/iphoneos/*.m src/loadso/dlopen/*.c src/power/uikit/*.m
        src/thread/pthread/*.c src/timer/unix/*.c src/video/uikit/*.m src/video/uikit/*.c src/haptic/dummy/*.c
        src/filesystem/cocoa/*.m
    )
elseif (MAC)
    file (GLOB SYS_C_FILES
        src/audio/coreaudio/*.c src/file/cocoa/*.m src/haptic/darwin/*.c src/joystick/darwin/*.c src/loadso/dlopen/*.c
        src/power/macosx/*.c src/thread/pthread/*.c src/timer/unix/*.c src/video/cocoa/*.m src/filesystem/cocoa/*.m
    )
elseif (ANDROID)
    add_definitions(-DGL_GLEXT_PROTOTYPES)
    file (GLOB SYS_C_FILES
        src/audio/android/*.c 
        src/audio/dummy/*.c 
        src/core/android/*.c 
        src/joystick/android/*.c 
        src/loadso/dlopen/*.c 
        src/power/*.c
        src/power/android/*.c
        src/thread/pthread/*.c 
        src/timer/unix/*.c 
        src/video/android/*.c 
        src/haptic/dummy/*.c 
        src/filesystem/dummy/*.c
        src/filesystem/android/*.c
    )
endif()
macro (setup_library name)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
    include_directories(${INCLUDE_DIRS})
    add_library(${name} STATIC ${SOURCE_FILES})
endmacro()

file (GLOB H_FILES include/*.h)

set (SOURCE_FILES ${C_FILES} ${SYS_C_FILES} ${H_FILES})

set (INCLUDE_DIRS include)

foreach(src_file ${SOURCE_FILES})
    get_filename_component (PATH ${src_file} PATH)
    string (REPLACE ${CMAKE_CURRENT_SOURCE_DIR} "" PATH ${PATH})
    string (REPLACE / \\ PATH ${PATH})
    source_group ("Source Files\\${PATH}" FILES ${src_file})
endforeach()

add_definitions(-DHAVE_LIBC=1)

setup_library (SDL2-static)

if (WIN32)
    target_link_libraries(SDL2-static user32 gdi32 winmm imm32 ole32 oleaut32 version uuid)
elseif(ANDROID)
    target_link_libraries(SDL2-static dl log android GLESv1_CM GLESv2)
endif()


if (ANDROID)
    set (SOURCE_FILES src/main/android/SDL_android_main.c)
elseif (WIN32)
    set (SOURCE_FILES src/main/windows/SDL_windows_main.c)
endif()

set (INCLUDE_DIRS include)
setup_library (SDL2main)



