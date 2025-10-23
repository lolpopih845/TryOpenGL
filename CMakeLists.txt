cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

set(LIB_ROOT "C:/OpenGL")

# === Source files ===
file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        include/**/*.cpp
        include/**/*.h
        include/*.cpp
        include/*.h
        MeshPlayer.h
        HappyForm.h
)

# === Include directories ===
configure_file(config/root_directory.h.in config/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/config ${LIB_ROOT}/include)

# === Link directory for GLFW ===
#link_directories(${LIB_ROOT}/glfw/build/src/Release)

# === Add glad as a static library ===
add_library(glad STATIC ${LIB_ROOT}/glad/src/glad.c

)

# === Executable ===
add_executable(OpenGLProject ${SOURCE_FILES}
)

# Copy shaders folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# === Link all libraries ===
target_link_directories(OpenGLProject PRIVATE
        ${LIB_ROOT}/glfw/build/src
        ${LIB_ROOT}/assimp/build/lib
)
target_link_libraries(OpenGLProject glad glfw3 opengl32 assimp)

add_custom_command(TARGET OpenGLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_ROOT}/assimp/build/bin/libassimp-6.dll"
        $<TARGET_FILE_DIR:OpenGLProject>
)