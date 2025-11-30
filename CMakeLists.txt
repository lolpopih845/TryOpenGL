cmake_minimum_required(VERSION 3.10)
project(OpenGLProject)

set(CMAKE_CXX_STANDARD 17)

# Root directory for your libs
set(LIB_ROOT "C:/Users/HP/Desktop/Ug/Y3/GE/OpenGL")

# === Source files ===
file(GLOB_RECURSE SOURCE_FILES
        main.cpp
        include/**/*.cpp
        include/**/*.h
        HappyCat/*.cpp
        HappyCat/*.h
        HappyCat/**/*.cpp
        HappyCat/**/*.h
        MeshPlayer.h
        HappyForm.h
)

# === Include directories ===
configure_file(config/root_directory.h.in config/root_directory.h)
include_directories(
        ${CMAKE_BINARY_DIR}/config
        ${LIB_ROOT}/includes
)

# === glad as STATIC lib ===
add_library(glad STATIC ${LIB_ROOT}/glad.c)

# === Executable ===
add_executable(OpenGLProject ${SOURCE_FILES})

# === Link directories (MSVC .lib folders) ===
target_link_directories(OpenGLProject PRIVATE
        ${LIB_ROOT}/lib
)

# === Link libraries (.lib names without extension) ===
target_link_libraries(OpenGLProject
        glad
        glfw3
        opengl32
        assimp
        freetype
        irrKlang
)

# === Copy DLLs after build (if they exist) ===
add_custom_command(TARGET OpenGLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_ROOT}/dlls/assimp-vc140-mt.dll"
        $<TARGET_FILE_DIR:OpenGLProject>
)

# Copy IrrKlang DLL
add_custom_command(TARGET OpenGLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_ROOT}/dlls/irrKlang.dll"
        $<TARGET_FILE_DIR:OpenGLProject>
)

add_custom_command(TARGET OpenGLProject POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
        "${LIB_ROOT}/dlls/ikpMP3.dll"
        $<TARGET_FILE_DIR:OpenGLProject>
)