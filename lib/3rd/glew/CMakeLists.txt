
file (GLOB SOURCE_FILES *.c)

set (INCLUDE_DIRS ${CMAKE_CURRENT_SOURCE_DIR}/include)

find_package (OpenGL REQUIRED)
set (GLEW_LIBRARIES ${OPENGL_LIBRARIES})

set(CMAKE_DEBUG_POSTFIX d)

add_definitions (-DGLEW_NO_GLU)

library (glew)
set_target_properties (glew PROPERTIES COMPILE_DEFINITIONS "GLEW_STATIC" OUTPUT_NAME "${GLEW_LIB_NAME}" PREFIX lib)
target_link_libraries (glew ${GLEW_LIBRARIES})
target_compile_definitions(glew INTERFACE "GLEW_STATIC")

set_target_properties(glew PROPERTIES FOLDER "3rd")