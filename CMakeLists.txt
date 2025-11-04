cmake_minimum_required(VERSION 3.29)
project(CSC271)
set(CMAKE_CXX_STANDARD 17)

# Add include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/include
        ${CMAKE_SOURCE_DIR}/glad/include
        ${CMAKE_SOURCE_DIR}/include
        ${CMAKE_SOURCE_DIR}/glm-1.0.1/glm
)

# Add library directories
link_directories(
        ${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64
)

# Add GLAD source file
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

# Define the executable
add_executable(CSC271
        main.cpp
        src/shaderprogram.cpp
        src/stb_image.cpp
        src/mesh.cpp
        src/tiny_obj_loader.cpp
        src/camera.cpp
)

# Compile-time definitions (shaders and assets directories)
add_compile_definitions(
        SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/"
        ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/"
)

# Link libraries
target_link_libraries(CSC271
        glad
        glfw3
        opengl32
)

