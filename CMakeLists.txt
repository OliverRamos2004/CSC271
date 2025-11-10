# Specify the minimum required version of CMake to process this file
cmake_minimum_required(VERSION 3.27)

# Declare the name of the project
project(CSC271)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Include directories
include_directories(
        ${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/include
        ${CMAKE_SOURCE_DIR}/glad/include
        include
        ${CMAKE_SOURCE_DIR}/glm-1.0.1
        ${CMAKE_SOURCE_DIR}/glm-1.0.1/glm
)

# Link directories
link_directories(${CMAKE_SOURCE_DIR}/glfw-3.4.bin.WIN64/lib-mingw-w64)

# Add GLAD source file
add_library(glad ${CMAKE_SOURCE_DIR}/glad/src/glad.c)

# Define the executable target (make sure main.cpp is included!)
add_executable(CSC271
        main.cpp
        src/shaderprogram.cpp
        src/stb_image.cpp
        src/mesh.cpp
        src/tiny_obj_loader.cpp
        src/camera.cpp
)

# Compile-time definitions for directories
add_compile_definitions(SHADER_DIR="${CMAKE_SOURCE_DIR}/shaders/")
add_compile_definitions(ASSETS_DIR="${CMAKE_SOURCE_DIR}/assets/")

# Link the libraries
target_link_libraries(CSC271
        glad
        glfw3
        opengl32
)
