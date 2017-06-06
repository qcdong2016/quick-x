
add_subdirectory(imgui)
add_subdirectory(kazmath)
add_subdirectory(lua)
add_subdirectory(lua_extensions)
add_subdirectory(SDL2)
add_subdirectory(spine)
add_subdirectory(stb)
add_subdirectory(tinyxml2)
add_subdirectory(tolua)

if (MSVC)
    add_subdirectory(zlib)
    add_subdirectory(glew)
endif()

set(ThirdLib 
    imgui
    kazmath
    lua
    lua_extensions
    SDL2
    spine
    stb
    tinyxml2
    tolua
    zlib
    glew
)
set_target_properties(${ThirdLib} PROPERTIES FOLDER "3rd")
