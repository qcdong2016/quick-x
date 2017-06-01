
file (GLOB SOURCE_FILES *.c)

set (INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package (OpenGL REQUIRED)
set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

set(CMAKE_DEBUG_POSTFIX d)

add_definitions (-DGLEW_NO_GLU)

library (GLEW)
set_target_properties (GLEW PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)
target_link_libraries (GLEW ${GLEW_LIBRARIES})
target_compile_definitions(GLEW INTERFACE "GLEW_STATIC")