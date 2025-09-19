cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# === Base path to all external libraries ===
set(LIB_ROOT "C:/OpenGL")

# === Include directories ===
configure_file(config/root_directory.h.in config/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/config)

include_directories(
        ${LIB_ROOT}/glfw/include
        ${LIB_ROOT}/GLAD/include
        ${LIB_ROOT}/glm/include
        ${LIB_ROOT}/stb
        ${LIB_ROOT}/assimp/include
)

# === Link directory for GLFW ===
#link_directories(${LIB_ROOT}/glfw/build/src/Release)

# === Add glad as a static library ===
add_library(glad STATIC ${LIB_ROOT}/glad/src/glad.c
        include/shader.h
        include/shader.cpp
        include/texture.cpp
        include/texture.h
        include/FileSystem.h
        include/stb_image.cpp
        include/camera.cpp
        include/camera.h
        include/Mesh.cpp
        include/Mesh.h
        include/model.cpp
        include/model.h
)

# === Executable ===
add_executable(OpenGLProject main.cpp
        MeshPlayer.h
        HappyForm.h
)

# Copy shaders folder to build directory
file(COPY ${CMAKE_SOURCE_DIR}/shaders DESTINATION ${CMAKE_BINARY_DIR})

# === Link all libraries ===
target_link_directories(OpenGLProject PRIVATE ${LIB_ROOT}/glfw/build/src)
target_link_libraries(OpenGLProject glad glfw3 opengl32)
